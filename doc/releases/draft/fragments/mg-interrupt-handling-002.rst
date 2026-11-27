::area:: Interrupt Handling
::pr:: 112001

::migration-guide::
Deprecate ``GIC_NUM_CPU_IF`` from GIC header file :file:`gic.h`. One shall use
instead.:kconfig:option:`CONFIG_MP_MAX_NUM_CPUS` instead.
