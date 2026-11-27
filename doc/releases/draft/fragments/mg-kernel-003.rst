::area:: Kernel
::pr:: 115174

::migration-guide::
The ``CONFIG_SMP_BOOT_DELAY`` Kconfig option has been removed. Deferring the start of secondary
CPUs to run time is now expressed per CPU in the devicetree: add the ``zephyr,deferred-start``
flag to the corresponding ``cpu`` node under ``/cpus`` (typically in a board overlay) and start
the CPU later with :c:func:`k_smp_cpu_start` or :c:func:`k_smp_cpu_resume`, as before. Unlike
the removed option, which skipped every secondary CPU, deferral can now be chosen for each CPU
individually. Note that the flag only takes effect on cpu nodes whose devicetree binding
includes ``cpu.yaml``; a node without such a binding cannot be deferred.
