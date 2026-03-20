.. _workq_fifo:

FIFO Work Queue
###############

.. contents::
  :local:
  :depth: 2

Overview
********

A FIFO work queue defers work from interrupt context or a thread to one or more worker threads which
processes the work in submission order.

The queue owns the state of the work submitted to it. A work item has no state without a
relationship to a queue. This has a number of consequences that shape how the API is used:

* **Lifecycle management**: because the state is derived from the queue, a work item may be safely
  freed or re-submitted from inside its own callback. This makes *fire-and-forget* dynamically
  allocated work items practical.
* **Parallel execution**: the queue is decoupled from the worker thread, so several worker threads
  may process one queue at the same time. Items are always dequeued in submission order, but
  completion order may vary with callback duration, thread priority and scheduling.
* **Unified work items**: :c:struct:`workq_fifo_work` is used for both immediate and delayed work,
  which allows user to unify behaviour to a single callback.
* **Deterministic teardown**: the queue holds a reference to every submitted item, including delayed
  ones, so a queue can be stopped, frozen and reclaimed in a well-defined order.

Concepts and Structures
***********************

The API is built from four structures:

* :c:struct:`workq_fifo`: the queue itself. It tracks the items that are ready to run, the items
  waiting for their submission time, and the items currently running, together with the single
  timeout used to schedule delayed work.
* :c:struct:`workq_fifo_work`:  a unit of work.
* :c:struct:`workq_thread`: an optional worker thread bound to a queue. Several worker threads may
  be bound to the same queue.
* :c:struct:`workq_thread_config`: static configuration (name and priority) for a worker thread.

A work item is intrusive: it carries no pointer to your data. Embed it in an application-defined
struct and recover that struct with :c:macro:`CONTAINER_OF` inside the callback. The callback
receives a :c:struct:`work_base` pointer, which is the generic header embedded in every work item:

.. code-block:: c

   struct container {
           struct workq_fifo_work item;
           size_t number;
   };

   static void work_fn(struct work_base *base)
   {
           struct container *c = CONTAINER_OF(base, struct container, item.base);

           /* ... use c ... */
           k_free(c); /* safe: the item may be freed from its own callback */
   }


State Machines
**************

Work item
=========

A work item moves between *initialized*, *pending* (queued or delayed), and *running*. It cannot be
cancelled while running, but during the callback the item may be safely modified, freed, or
re-submitted.

.. mermaid::

   stateDiagram-v2
   [*] --> INITIALIZED : workq_fifo_work_init()

   INITIALIZED --> PENDING : workq_fifo_submit() / workq_fifo_delayed_submit()
   INITIALIZED --> PENDING : workq_fifo_reschedule()
   PENDING --> PENDING : workq_fifo_reschedule()

   PENDING --> INITIALIZED : workq_fifo_cancel()
   PENDING --> RUNNING : workq_fifo_run()

   RUNNING --> PENDING : [re-submitted in callback]
   RUNNING --> INITIALIZED : [not re-submitted]

   note right of RUNNING
     Cannot be cancelled.
     The work item can be safely modified, freed, or re-submitted.
   end note

Queue
=====

A queue is either ``OPEN`` (accepting new work) or ``CLOSED`` (rejecting it), and orthogonally
either ``RUNNING`` or ``FROZEN``.
:c:func:`workq_fifo_open` / :c:func:`workq_fifo_close` control only whether outside submissions are
accepted, and :c:func:`workq_fifo_freeze` / :c:func:`workq_fifo_thaw` control only whether delayed
items are scheduled. Freezing keeps processing pending items but stops scheduling delayed items
until the queue is thawed, without affecting whether new work is accepted.

.. mermaid::

   stateDiagram-v2
   [*] --> OPEN : workq_fifo_init()

   state "OPEN" as OPEN {
     [*] --> OPEN_RUNNING
     OPEN_RUNNING
     OPEN_FROZEN
     OPEN_RUNNING --> OPEN_FROZEN : workq_fifo_freeze()
     OPEN_FROZEN --> OPEN_RUNNING : workq_fifo_thaw()

     note right of OPEN_RUNNING
         Accepting new work.
         Processing pending and scheduling delayed items.
     end note

     note right of OPEN_FROZEN
         Accepting new work.
         Pending items run, but delayed items are
         not scheduled until thawed.
     end note
   }

   state "CLOSED" as CLOSED {
     CLOSED_RUNNING
     CLOSED_FROZEN
     CLOSED_RUNNING --> CLOSED_FROZEN : workq_fifo_freeze()
     CLOSED_FROZEN --> CLOSED_RUNNING : workq_fifo_thaw()

     note right of CLOSED_RUNNING
         Rejecting new work (-EAGAIN).
         Processing pending and scheduling delayed items.
     end note

     note right of CLOSED_FROZEN
         Rejecting new work (-EAGAIN).
         Pending items run, but delayed items are
         not scheduled until thawed.
     end note
   }

   OPEN_RUNNING --> CLOSED_RUNNING : workq_fifo_close()
   CLOSED_RUNNING --> OPEN_RUNNING : workq_fifo_open()
   OPEN_FROZEN --> CLOSED_FROZEN : workq_fifo_close()
   CLOSED_FROZEN --> OPEN_FROZEN : workq_fifo_open()

Worker thread
=============

A worker thread is an optional entity in the scheme of the workq. It is bound to a workqueue and
acts as the execution context for the work items.
The workq_fifo_thread is a ease of use wrapper but it isn't a requirement for the workq_fifo to
function. The workq_fifo_run() function can be called from any thread context to process work items.
This is particularly useful for memory constrained systems where reusing the main thread to process
work items is preferred over creating a dedicated worker thread. Or if a single thread can be used
to process multiple workqueues.

.. mermaid::

   stateDiagram-v2
   [*] --> UNALLOCATED

    UNALLOCATED --> INITIALIZED : workq_fifo_thread_init()
    UNALLOCATED --> RUNNING : WORKQ_FIFO_THREAD_DEFINE() (static)

    INITIALIZED --> RUNNING : workq_fifo_thread_start()

    RUNNING --> INITIALIZED : workq_fifo_thread_stop()
    RUNNING --> INITIALIZED : workq_fifo_run() error (self-exit)

    note right of INITIALIZED
         The work queue thread is initialized but not running.
    end note
    note right of RUNNING
         The work queue thread is running and can process work items.
    end note

Instantiation and Usage
***********************

A queue can be defined statically with :c:macro:`WORKQ_FIFO_DEFINE` or initialized at runtime with
:c:func:`workq_fifo_init`.
Worker threads are defined with :c:macro:`WORKQ_FIFO_THREAD_DEFINE`, or created at runtime with
:c:func:`workq_fifo_thread_init` followed by :c:func:`workq_fifo_thread_start`.
A thread defined with :c:macro:`WORKQ_FIFO_THREAD_DEFINE` starts in the running state and does not
require an explicit :c:func:`workq_fifo_thread_start`.

For finer control over thread configuration, :c:macro:`WORKQ_THREAD_CONFIG` defines a
:c:struct:`workq_thread_config` statically, :c:macro:`WORKQ_THREAD_CONFIG_INITIALIZER` provides an
initializer for one embedded in another structure, and :c:macro:`WORKQ_FIFO_THREAD_INITIALIZE`
initializes a :c:struct:`workq_thread` from an existing config, stack, and queue.

.. code-block:: c

   #include <zephyr/workqs/fifo.h>

   #define PRIORITY 0

   WORKQ_FIFO_DEFINE(my_workq);
   WORKQ_FIFO_THREAD_DEFINE(worker1, my_workq, 1024, PRIORITY);
   WORKQ_FIFO_THREAD_DEFINE(worker2, my_workq, 1024, PRIORITY);

A work item is initialized with :c:func:`workq_fifo_work_init` and submitted
with :c:func:`workq_fifo_submit`:

.. code-block:: c

   struct container *c = k_malloc(sizeof(*c));

   workq_fifo_work_init(&c->item, work_fn);
   c->number = 0;
   workq_fifo_submit(&my_workq, &c->item);


Multiple Worker Threads
=======================

Binding several :c:struct:`workq_thread` instances to one queue lets I/O-bound work be processed in
parallel. Items are dequeued in submission order, but because callbacks may run for different
durations, their completion order is not guaranteed.

Delayed Work
============

Delayed work items are managed by the queue itself, which keeps a single timeout for the earliest
delayed item rather than a timer per item.
When an item's execution time is reached it is moved to the ready list. Because the queue tracks
every delayed item, they are also visible to teardown.

Open/Close and Freeze/Thaw
==========================

* :c:func:`workq_fifo_open` / :c:func:`workq_fifo_close` control whether the queue accepts new
  outside submissions. A closed queue still processes work already queued.
* :c:func:`workq_fifo_freeze` sets the frozen state and aborts the delayed-work timeout, so delayed
  items are not promoted to the ready list.
  :c:func:`workq_fifo_thaw` clears the frozen state and reschedules delayed
  work.

Freeze/thaw are orthogonal to open/close: they do not change whether the queue accepts new
submissions, and a delayed item submitted while frozen is simply queued and scheduled on thaw.

Deterministic Teardown
======================

Because the queue references every submitted item, a subsystem can be torn down deterministically:

#. Stop the worker thread(s) with :c:func:`workq_fifo_thread_stop`. This
   function takes a ``timeout`` and returns the result of joining the thread
   (for example ``-EAGAIN`` if the join times out).
#. :c:func:`workq_fifo_close` the queue to prevent new submissions, and
   :c:func:`workq_fifo_freeze` it to stop delayed scheduling.
#. Reclaim any resources associated with the work items and the queue.

.. _workq_fifo_synchronization:

Synchronization
===============

There is no ``sync()`` primitive. Because a work item may free or re-queue itself in its own
callback, a waiting thread cannot reliably distinguish the original task from a re-queued or newly
allocated item that reuses the same address.

When a thread must block until a specific work item has finished, the item should carry its own
synchronization object, such as a semaphore, and the callback should signal it:

.. code-block:: c

   struct sync {
           struct workq_fifo_work item;
           struct k_sem sem;
   };

   static void sync_fn(struct work_base *base)
   {
           struct sync *s = CONTAINER_OF(base, struct sync, item.base);

           /* ... do work ... */
           k_sem_give(&s->sem);
   }

   void submit_and_wait(void)
   {
           struct sync *s = k_malloc(sizeof(*s));

           workq_fifo_work_init(&s->item, sync_fn);
           k_sem_init(&s->sem, 0, 1);
           workq_fifo_submit(&my_workq, &s->item);
           k_sem_take(&s->sem, K_FOREVER);
           k_free(s); /* the item outlives the callback; free it here */
   }

A complete, runnable example is available in the :zephyr:code-sample:`workq-fifo` sample.

API Reference
*************

.. doxygengroup:: workq_fifo_apis

.. doxygengroup:: workq_common_apis
