.. _call_offloading:

Call Offloading
###############

.. contents::
    :local:
    :depth: 1

Some APIs are, in relative terms, stack-hungry; cryptographic libraries are a common example.
Without mitigation, every thread that calls such an API must size its own stack for the deepest
call it makes.

A :dfn:`call offloader` is a pool of worker threads fed by a priority-sorted queue of pending
callers. A caller hands a function and an argument blob to :c:func:`offloader_dispatch` and blocks
until a worker has run it, so the deep stack is paid once per worker instead of once per calling
thread.

Unlike a :ref:`workqueue <workqueues_v2>`, which runs work items asynchronously on a thread of
fixed priority, offloading is synchronous and the worker runs the function at the caller's
priority.

Usage
*****

Enable :kconfig:option:`CONFIG_OFFLOADER`, then declare an engine together with its worker pool
using :c:macro:`OFFLOADER_ENGINE_DEFINE`.

.. code-block:: c

   #include <zephyr/offloader/engine.h>

   OFFLOADER_ENGINE_DEFINE(my_engine, 1, 3072);

Hand a function and an argument blob to :c:func:`offloader_dispatch`. The call blocks until a
worker has run the function and returns.

.. code-block:: c

   offloader_dispatch(&my_engine, my_work_fn, &my_args);

Keep the following constraints in mind:

* :c:func:`offloader_dispatch` must not be called from an ISR. The assertion guarding this is
  build-configurable, but the underlying pend refuses ISR context with ``k_panic()`` in every
  build, so a violation is a fatal error rather than a silently ignored one.

* :c:macro:`OFFLOADER_ENGINE_DEFINE` starts the workers from a ``SYS_INIT()`` entry at
  ``POST_KERNEL`` priority 1. A call dispatched before that entry runs finds no worker, queues
  itself and blocks forever. There is no assertion for this case: it presents as a silent hang.

* The argument blob must stay valid for the duration of the call. :c:type:`offloader_work_fn_t`
  returns ``void``, so results travel back to the caller only through fields of that blob.

* The caller pends with ``K_FOREVER``. There is no timeout, no cancellation and no bound on the
  pending queue; a work function that hangs wedges its caller permanently.

* Each worker stack must accommodate the deepest offloaded call, where "deepest" includes any
  same-engine dispatches nested inside it: the inline escape described below runs the nested
  function on the worker's stack, inside the frame of the work function already running there.

* The API is kernel-mode only. :c:func:`offloader_dispatch` is not a system call; it dereferences
  internal thread state and takes a kernel spinlock, and the engine object the macro defines is
  static kernel memory. A user thread cannot reach the engine, and calling the function from user
  mode faults. The workers themselves are created with no options on stacks from
  ``K_KERNEL_STACK_ARRAY_DEFINE()``, so they are plain kernel threads.

* Every call costs two context switches on both paths, plus a spinlock round trip. The queued
  path adds the promotion pass: a walk of the pending queue down to the caller's priority, and a
  scan of the whole worker array, which dominates when the queue is short. The worker's
  reschedule after it adopts the client's priority can add a third switch.

* The macro defines the engine object ``static``, so an engine is reachable only from the file
  that defines it. Export a wrapper function, not the engine.

Sizing the pool
***************

``_num_threads`` sets how many offloaded calls can run concurrently; ``_stack_size`` must cover
the deepest call made through the engine. The pool costs ``_num_threads * _stack_size`` bytes
plus the thread objects, against the stack each caller no longer has to reserve. A single worker
is the cheapest configuration and serializes all callers; adding workers buys concurrency and
shortens queueing delays at a full stack each.

Size ``_stack_size`` from measurement. :kconfig:option:`CONFIG_THREAD_ANALYZER` reports the
high-water mark of every thread and selects the stack bookkeeping it needs, but it depends on
``!ARCH_POSIX``, so it is unavailable on ``native_sim``. Note also that
:c:macro:`OFFLOADER_ENGINE_DEFINE` gives every worker of an engine the same thread name, so a
multi-worker pool appears as several indistinguishable rows; read them as a set rather than
trying to attribute one to a particular call.

:c:func:`k_thread_stack_space_get` reports the *unused* space on one thread, the complement of
that high-water mark, and requires :kconfig:option:`CONFIG_INIT_STACKS` and
:kconfig:option:`CONFIG_THREAD_STACK_INFO`. The worker array is internal and static, so this is
practical only from inside the file that defines the engine.

Design
******

An engine owns a priority-sorted ``pending`` wait queue of callers and an ``idle`` wait queue of
workers; both drive dispatch. A third ``running`` wait queue holds the client while its work runs,
but nothing in tree reads it, so it is bookkeeping only.

:c:func:`offloader_dispatch` on a thread that is not a worker of this engine:

#. Packs ``fn`` and ``args`` into a request on the caller's own stack and publishes a pointer to
   it in the caller's thread state.
#. Takes the engine lock and tries to wake one worker from ``idle``.
#. If no worker was idle, runs the promotion pass described below.
#. Pends the caller on ``pending`` with ``K_FOREVER``, releasing the engine lock. This happens on
   **both** paths: waking an idle worker is not a direct hand-off, and the caller is always
   queued. The woken worker unpends it from ``pending`` in the next step.

One iteration of a worker's loop:

#. Takes the engine lock.
#. Under the scheduler lock: if it still holds a client from the previous iteration, sets that
   client's return value and readies it; then unpends the highest-priority caller from
   ``pending`` as its new client and adds it to ``running``.
#. If there was no caller to take, drops to its idle priority and pends on ``idle``, releasing the
   engine lock.
#. Otherwise sets its own priority to the client's, reads the request pointer from the client's
   thread state, and reschedules, which also releases the engine lock.
#. Calls ``fn(args)``, outside the engine lock. This is what lets the other workers dispatch
   concurrently.

The client is therefore moved to ``running`` *before* its function runs, and it is woken at the
top of the worker's next iteration, not when ``fn`` returns. Between the return and the wake the
worker must re-acquire the engine lock, so contention on that lock delays the client's wake.

Priority handling:

* A worker idles at ``K_PRIO_COOP(0)``, the highest cooperative priority. Idle workers are
  therefore cooperative threads, and a worker woken from ``idle`` runs at that priority until it
  adopts its client's priority in step 4, preempting essentially every application thread in that
  window.

* The promotion pass raises the *lowest*-priority busy worker to the new caller's priority. It
  does so only when the pending callers at or above the new caller's priority, counting the new
  caller itself, outnumber the busy workers currently at or above it.

* At most one promotion is issued per dispatch, however large the deficit. Three high-priority
  callers arriving against three low-priority busy workers promote one worker; the shortfall is
  not re-evaluated until the next dispatch.

* The count of workers "already serving at or above" uses each worker's *current* priority, which
  may itself be the result of an earlier promotion, not the priority of the work it is running. A
  worker promoted earlier inflates that count, so a later high-priority caller can see an
  apparently adequate pool and get no promotion of its own.

* Promotion raises work already in flight, which then temporarily executes at the higher priority.
  It is priority inheritance applied to running work, not a hand-off of the pending request.

Ordering and concurrency:

* Callers are served by priority. Within a priority they are served in submission order. Under
  :kconfig:option:`CONFIG_SCHED_DEADLINE` the wait queue compares deadlines after priority, so
  submission order holds only among callers that have not set one.

* With more than one worker, calls run concurrently. The offloaded API must be reentrant, and
  concurrent dispatches have no ordering guarantee relative to each other.

* Dispatching from a thread that is itself a worker of the same engine calls the function inline
  on that worker's stack rather than queueing it. This escape is unconditional on pool size.
  Queueing would deadlock outright on a single-worker engine, or whenever every other worker is
  also blocked; with spare idle workers it would merely occupy one, so the escape also avoids
  wasting a worker.

* The escape covers the same engine only. A worker that dispatches to a second engine stays
  blocked and keeps occupying a worker of the first; a dispatch cycle between two single-worker
  engines deadlocks.

Offloading an API
*****************

To offload an API, define an engine, pack each call's arguments and its return value into a
struct, unpack them in a work function, and wrap it in a thin function that dispatches and
returns the result (if any).

.. code-block:: c

   #include <zephyr/offloader/engine.h>

   OFFLOADER_ENGINE_DEFINE(my_engine, 1, 3072);

   struct do_thing_args {
   	const uint8_t *input;
   	size_t input_len;
   	int ret;
   };

   static void do_thing(void *args)
   {
   	struct do_thing_args *a = args;

   	a->ret = thing(a->input, a->input_len);
   }

   int offloaded_thing(const uint8_t *input, size_t input_len)
   {
   	struct do_thing_args a = {
   		.input = input,
   		.input_len = input_len,
   	};

   	offloader_dispatch(&my_engine, do_thing, &a);

   	return a.ret;
   }

:zephyr_file:`tests/lib/offloader` exercises this pattern in tree.

API Reference
*************

.. doxygengroup:: offloader
