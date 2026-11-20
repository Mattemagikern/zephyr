.. _call_offloading:

Call Offloading
###############

.. contents::
    :local:
    :depth: 1

Some APIs are, in relative terms, stack-hungry. Without mitigation, every thread that calls such an
API must size its own stack for the deepest call it makes.

A :dfn:`call offloader` is a set of stack slots. A caller hands a function and an argument blob to
:c:func:`call_offloader_dispatch`, which starts a thread on a free slot to run it and blocks until
that thread has finished, so the deep stack is paid once per slot instead of once per calling
thread.

Rationale
*********

Stack sizing is per-thread and worst-case. Every thread that might call a stack-hungry API has to
reserve the deepest call it can reach, even though only a few of those calls are ever in flight at
once. A system with many such callers therefore reserves the deep stack once per thread and leaves
nearly all of it idle. On a constrained device that reservation, rather than the code itself, is
what runs out.

The obvious alternatives each give something up:

* Enlarging every caller's stack is exactly the per-thread reservation described above, repeated
  for each caller.

* Moving the work to a :ref:`workqueue <workqueues_v2>` concentrates the deep stack onto one
  thread, but changes how the work is called: it runs asynchronously, at the queue's fixed
  priority, and cannot return a value inline. Synchronous call sites have to be restructured into
  submit-and-wait form, and the work no longer tracks the caller's priority.

An offloader concentrates the stack the same way while keeping the call synchronous, returning its
result inline, and running it at the caller's priority. The API behind it has to be wrapped, as
shown below, but its call sites do not have to change.

The saving is the difference between reserving the deep stack for every caller and reserving it
for every slot, so offloading pays off when callers substantially outnumber slots. That is the
usual shape for a shared, stack-hungry service. What it costs is the per-call overhead and the
constraints below.

Usage
*****

Enable :kconfig:option:`CONFIG_CALL_OFFLOADER`, then declare an engine together with its stack
slots using :c:macro:`CALL_OFFLOADER_ENGINE_DEFINE`.

.. code-block:: c

   #include <zephyr/call_offloader/call_offloader.h>

   CALL_OFFLOADER_ENGINE_DEFINE(my_engine, 1, 3072);

Hand a function and an argument blob to :c:func:`call_offloader_dispatch`. The call blocks until a
thread on one of the engine's slots has run the function and returns.

.. code-block:: c

   call_offloader_dispatch(&my_engine, my_work_fn, &my_args);

Keep the following constraints in mind:

* :c:func:`call_offloader_dispatch` must not be called from an ISR or before the kernel has
  started. Both are guarded by build-configurable assertions. No thread exists until a call is
  dispatched, so an engine needs no initialization of its own and can be used from any
  ``POST_KERNEL`` initialization priority.

* The argument blob must stay valid for the duration of the call.

* The caller pends with ``K_FOREVER``. There is no timeout, no cancellation and no bound on the
  number of waiting callers.

* A fatal error inside an offloaded call kills the engine thread running it, not the caller, so
  the application's :c:func:`k_sys_fatal_error_handler` sees the engine thread as the faulting
  thread. The caller then frees the slot and aborts itself without raising a second fatal error,
  so one fault produces one report. State the offloaded call left behind, such as a library mutex
  it held, is not recovered; a direct call that faults the same way leaves the same wreckage. With
  the default handler the system halts on the first fault and none of this runs.

* A thread blocked in :c:func:`call_offloader_dispatch` must not be aborted. The kernel neither
  frees nor overwrites its stack on abort, but the engine thread is still reading the argument
  blob from it and writes its completion into it, so reusing that stack or thread object before
  the call finishes corrupts it. The slot itself is freed by the engine thread and is not lost.

* Engine threads are not the application's to abort. One aborted after finishing its call but
  before freeing its slot leaves that slot unavailable for good.

* Each slot stack must accommodate the deepest offloaded call, where "deepest" includes any
  same-engine dispatches nested inside it, which run inline on the slot's own stack.

* The API is kernel-mode only. :c:func:`call_offloader_dispatch` is not a system call; it
  dereferences internal thread state and takes a kernel spinlock, and the engine object the macro
  defines is static kernel memory. A user thread cannot reach the engine, and calling the function
  from user mode faults. The engine threads are created with no options on stacks from
  ``K_KERNEL_STACK_ARRAY_DEFINE()``, so they are plain kernel threads.

* Every call creates, starts, joins and tears down a thread, on top of the two context switches.
  Thread creation runs every per-thread setup step the build enables: with
  :kconfig:option:`CONFIG_INIT_STACKS` that includes filling the whole slot stack, and with
  :kconfig:option:`CONFIG_THREAD_LOCAL_STORAGE` copying the TLS image. When no slot is free the
  call also pays the promotion pass: a walk of the waiting callers and a scan of the slots. A
  dispatch that takes the inline escape described below pays none of this.

Sizing the engine
*****************

``_num_slots`` sets how many offloaded calls can run concurrently; ``_stack_size`` must cover the
deepest call made through the engine. The engine costs ``_num_slots * _stack_size`` bytes plus the
thread objects, against the stack each caller no longer has to reserve. A single slot is the
cheapest configuration and serializes all callers. Adding slots buys concurrency at the cost of
more RAM.

Every call starts a fresh thread, so a stack high-water mark covers one call only. With
:kconfig:option:`CONFIG_INIT_STACKS` the slot stack is refilled on each creation, and the usage of
earlier calls is lost.

Design
******

A dispatch claims a free slot, or queues the caller until one is handed to it. It then joins the
slot's previous thread, creates a new thread on the slot at the caller's priority, and joins that
one. The thread runs the function, marks the request done and releases its slot before it exits.
Releasing hands the slot directly to the first waiting caller, if there is one, so a slot is
passed on as soon as its call finishes, without waiting for the original caller to be scheduled
again. The new owner's join on the previous thread covers the short time that thread still needs
to exit.

Unlike a :ref:`workqueue <workqueues_v2>`, offloading is synchronous, and the engine thread runs
the function at the caller's priority or higher.

Priority handling:

* A slot thread is created at its caller's priority, so a call starts at exactly that priority.

* When no slot is free, the promotion pass raises the *lowest*-priority running slot thread to the
  new caller's priority. It does so only when the waiting callers at or above that priority,
  counting the new caller, outnumber the running slot threads currently at or above it, and only
  if some slot thread is actually running below the caller's priority. The priority is recorded
  on the slot as well as applied to its thread, so a slot whose thread is still being created
  picks up a promotion when its thread starts.

* A thread that hands its slot to a waiting caller raises itself to that caller's priority for
  the rest of its exit, so the caller's join on it is not held up by lower-priority work.

* Promotion raises work already in flight, which then temporarily runs at the higher priority. It
  is priority inheritance applied to running work, not a hand-off of the waiting request. A
  promotion ends with the thread it raised, since the next call on that slot creates a new one.

Ordering and concurrency:

* Callers waiting for a slot are served by priority, and within a priority in submission order.
  Deadlines set with :kconfig:option:`CONFIG_SCHED_DEADLINE` play no part in that order.

* With more than one slot, calls run concurrently. The offloaded API must be reentrant, and
  concurrent dispatches have no ordering guarantee relative to each other.

* Dispatching from a thread that is itself running on a slot of the same engine calls the
  function inline on that slot's stack rather than waiting for another slot. Waiting would
  deadlock outright on a single-slot engine, or whenever every other slot is also blocked; with
  spare slots it would merely occupy one.

* That escape covers the same engine only. An engine thread that dispatches to a second engine
  blocks and keeps occupying a slot of the first, so a dispatch cycle between two single-slot
  engines deadlocks.

Offloading an API
*****************

To offload an API, define an engine and wrap each call with :c:macro:`CALL_OFFLOADER_WRAP`. The
macro takes the engine, the return type, the name of the wrapper and the parameters, each written
as the type in parentheses followed by the name. The body that follows is the code the engine runs
on one of its threads.

.. code-block:: c

   #include <zephyr/call_offloader/call_offloader.h>

   CALL_OFFLOADER_ENGINE_DEFINE(my_engine, 1, 3072);

   CALL_OFFLOADER_WRAP(my_engine, int, thing, (const uint8_t *) input, (size_t) input_len)
   {
           return thing(input, input_len);
   }

This defines ``call_offloader_thing()`` with the same signature as the body. Calling it packs the
arguments, blocks until an engine thread has run the body, and returns its result. The arguments
live on the caller's stack for the duration of the call, so a pointer parameter still refers to
memory the caller owns.

Parameter types whose declarator encloses the name, such as function pointer and array types,
cannot be written as ``(type) name`` and need a typedef. The return type must not be ``void``.

:zephyr_file:`tests/kernel/call_offloader` exercises this pattern in tree.

Redirecting call sites
======================

Writing the wrapper is only half the job: every caller still has to be changed to call
``call_offloader_thing()`` instead of ``thing()``. Giving the wrapper the *same signature* as the
original avoids that.

.. code-block:: c

   #include <thing.h>

   __typeof__(thing) call_offloader_thing;

   #ifndef THING_NO_REDIRECT
   #define thing call_offloader_thing
   #endif

Every ``thing(...)`` the compiler sees after that line resolves to the wrapper, with no edit to
the call site. Two rules make this work:

* The wrapper must be declared wherever it is called. When the redirect follows the real API
  header, as above, the ``__typeof__`` line declares it. When the redirect is in scope before the
  real API header is parsed, the macro renames the API's own prototype, which then declares the
  wrapper with the original signature. This only works for names the header declares as plain
  functions, not as macros or ``static inline`` functions.

* Build the file that *implements* the wrappers with the redirect suppressed, as the guard above
  allows. Without it, the body of ``call_offloader_thing()`` calls itself rather than the real
  ``thing()``.

Choosing the scope
==================

The redirect can be applied in three different scopes:

* a set of translation units
* a set of libraries
* globally

Including the redirect header file in a single source file, or in a small set of them, is the
simplest and most obvious way to offload a single call site. Include it after the real API header,
as described above, and the redirect is local to that translation unit.

When applying the redirect to a library it is easiest to pass the redirect as a compiler flag in
the library's build system. The redirect is then in scope for every translation unit of that
library, and every call the library makes to the API is offloaded. The rest of the build is
unaffected.

Using the same trick as for a library, the redirect can be applied to the entire build from the
top-level build system. Every translation unit sees the redirect, so every call to the API in the
build is offloaded with no source change at all. This scope comes with two extra requirements:

* The redirect **must** be suppressed in the translation unit that implements the wrapper, and in
  any that implement the original API. Without that, those definitions are rewritten into
  definitions of the wrapper name and the wrapper body calls itself, overflowing the slot stack.
  This is why a global redirect only fits an API whose implementation is a prebuilt or
  out-of-tree binary, or one whose every implementing library can be compiled with the guard
  macro defined.

* The redirect header must contain only macros and must not include any other header. A compiler
  flag places it before the first line of every translation unit, so any header it includes would
  be parsed ahead of the feature test macros, such as ``_POSIX_C_SOURCE``, that a source file
  defines before its own includes. Pass it with ``-imacros`` and rely on the renamed prototypes in
  the real API header to declare the wrappers, as described above.

Keep the following in mind:

* The redirect is textual and compiler-local. Calls made from a translation unit that does not
  see the ``#define``, including calls from inside a prebuilt library, keep reaching the original
  symbol.

* An object-like ``#define`` replaces *every* occurrence of the token, not just calls: a struct
  member, a local variable or a parameter that happens to share the name is rewritten too. A
  prefixed API makes this unlikely, but at whole-build scope the question covers the entire tree.
  A function-like ``#define thing(...) call_offloader_thing(__VA_ARGS__)`` replaces only calls,
  at the cost of no longer redirecting the address-of form below.

* Taking the address of the function yields the wrapper: ``&thing`` becomes
  ``&call_offloader_thing``. This is harmless while the signatures match, but it is visible if the
  address is stored in a table or compared against the original, and a redirected address
  registered as a callback will panic if that callback is ever invoked from an ISR.

* An engine thread that calls the redirected name re-enters the same engine and takes the inline
  escape described above rather than deadlocking.

API Reference
*************

.. doxygengroup:: call_offloader
