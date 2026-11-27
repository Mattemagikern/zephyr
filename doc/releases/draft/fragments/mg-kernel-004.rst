::area:: Kernel
::pr:: 110149

::migration-guide::
When :kconfig:option:`CONFIG_SCHED_CPU_MASK_PIN_ONLY` is enabled, calling
:c:func:`k_thread_cpu_mask_clear`, :c:func:`k_thread_cpu_mask_enable_all`,
or :c:func:`k_thread_cpu_mask_disable` now triggers an assertion instead of
silently producing an invalid state.  Applications using these functions
in PIN_ONLY mode must be updated to use :c:func:`k_thread_cpu_pin` instead.

:kconfig:option:`CONFIG_SCHED_CPU_MASK` is no longer restricted to
:kconfig:option:`CONFIG_SCHED_SIMPLE`.  Projects that previously selected
``SCHED_SCALABLE`` or ``SCHED_MULTIQ`` and worked around the limitation by
keeping ``SCHED_SIMPLE`` for affinity purposes can now use their preferred
backend directly.
