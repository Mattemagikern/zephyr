::area:: Kernel
::pr:: 117817

::migration-guide::
:c:func:`k_sem_reset` no longer wakes poll waiters waiting on the semaphore. Poll waiters
remain pending until the semaphore becomes available or the poll operation times out.
Applications that rely on reset to wake poll waiters must use an explicit synchronization
mechanism instead.
