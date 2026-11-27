::area:: Kernel
::pr:: 110149

::release-notes::
:kconfig:option:`CONFIG_SCHED_CPU_MASK` no longer depends on
:kconfig:option:`CONFIG_SCHED_SIMPLE`.  CPU affinity masks are now
supported on all three scheduler backends: ``SCHED_SIMPLE`` (O(N) list
scan), ``SCHED_SCALABLE`` (O(N) red/black tree walk), and ``SCHED_MULTIQ``
(O(P·N) per-priority bucket scan).  See the updated
:ref:`SMP documentation<smp_cpu_mask>` for per-backend performance notes.

:kconfig:option:`CONFIG_SCHED_CPU_MASK_PIN_ONLY` now enforces the
one-CPU-bit invariant at both the API boundary (``cpu_mask_mod()``) and at
queue time (``thread_runq()``).  Calling :c:func:`k_thread_cpu_mask_clear`,
:c:func:`k_thread_cpu_mask_enable_all`, or
:c:func:`k_thread_cpu_mask_disable` in PIN_ONLY mode triggers an assertion
failure.  Use :c:func:`k_thread_cpu_pin` to reassign a thread to a
different CPU.
