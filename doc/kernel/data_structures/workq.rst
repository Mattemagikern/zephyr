Work Queue (workq)
##################

The workq module provides a lightweight work queue for deferring tasks from
interrupt context or high-priority threads to one or more dedicated worker
threads.

Pending and delayed work items share a single bounded min-heap ordered by
execution time. The capacity of the heap is fixed at queue creation; submits
beyond capacity return ``-ENOMEM``.

Submission with :c:func:`workq_submit` queues an item for immediate execution
(``exec_time = now``) while :c:func:`workq_delayed_submit` queues it with a
deadline ``now + delay``. Either way the item is placed in the same heap and
is dispatched as soon as a worker becomes available and the deadline has
elapsed. Workers sleep until the earliest deadline rather than relying on a
shared kernel timeout.



State Machines
--------------


.. mermaid::

   stateDiagram-v2
   [*] --> INITIALIZED : work_init()

   INITIALIZED --> PENDING : workq_submit()
   PENDING --> PENDING : workq_reschedule()

   PENDING --> INITIALIZED : workq_cancel()
   PENDING --> RUNNING : workq_run()

   RUNNING --> PENDING: [pending]
   RUNNING --> INITIALIZED : [!pending] release_sync()

   note right of RUNNING
     Cannot be cancelled
     The work item can be safely modified or freed.
   end note


.. mermaid::

   stateDiagram-v2
   [*] --> OPEN : workq_init()

   state OPEN {
     [*] --> OPEN_RUNNING : [!frozen]
     [*] --> OPEN_FROZEN : [frozen]
     OPEN_RUNNING --> OPEN_FROZEN : workq_freeze()
     OPEN_FROZEN --> OPEN_RUNNING : workq_thaw()

     note right of OPEN_RUNNING
         Accepting new work.
         Processing items from the heap.
     end note

     note right of OPEN_FROZEN
         Rejecting new work (-EAGAIN).
         No items execute until thawed.
     end note
   }

   state CLOSED {
     [*] --> CLOSED_RUNNING : [!frozen]
     CLOSED_RUNNING --> OPEN_FROZEN : workq_freeze()

     note right of CLOSED_RUNNING
         Rejecting new work (-EAGAIN).
         Processing items already in the heap.
     end note
   }

   OPEN --> CLOSED : workq_close()
   CLOSED --> OPEN : workq_open()


.. mermaid::

   stateDiagram-v2
   [*] --> UNALLOCATED

    UNALLOCATED --> INITIALIZED : workq_thread_init()

    INITIALIZED --> RUNNING : workq_thread_start()

    RUNNING --> INITIALIZED: workq_thread_stop()


    note right of INITIALIZED
         The work queue thread is initialized but not running.
    end note
    note right of RUNNING
         The work queue thread is running and can process work items.
    end note

