::area:: Common Architecture Interface
::pr:: 116712

::migration-guide::
The RISC-V specific ``CONFIG_EXTRA_EXCEPTION_INFO`` has been removed. Use
:kconfig:option:`CONFIG_EXCEPTION_DEBUG` instead. The option is unchanged on Arm and SPARC.
