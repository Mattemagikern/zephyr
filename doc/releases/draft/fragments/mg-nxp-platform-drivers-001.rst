::area:: NXP Platform Drivers
::pr:: 115695

::migration-guide::
:kconfig:option:`CONFIG_MCUX_LPTMR_TIMER` no longer defaults to ``y`` based on the
``/chosen/zephyr,system-timer`` chosen node being compatible with
:dtcompatible:`nxp,lptmr`. Out-of-tree SoCs and boards that rely on the LPTMR
as the system timer must now explicitly default the symbol in their
``Kconfig.defconfig`` (for example ``default y if PM``).
