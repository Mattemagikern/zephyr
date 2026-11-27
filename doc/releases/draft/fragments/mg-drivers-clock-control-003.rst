::area:: Drivers: Clock control
::pr:: 113743

::migration-guide::
SiWx91x clock control has been split into three managers
(:dtcompatible:`silabs,siwx91x-cmu-aon`, :dtcompatible:`silabs,siwx91x-cmu-ulp`,
:dtcompatible:`silabs,siwx91x-cmu-hp`). The legacy :dtcompatible:`silabs,siwx91x-clock`
binding and ``clock0`` node are removed. Out-of-tree boards and overlays must update
``clocks`` phandles to the matching CMU and use the updated ``SIWX91X_CLK_*`` IDs from
``siwx91x-clock.h``. For example, ``clocks = <&clock0 SIWX91X_CLK_UART0>;`` becomes
``clocks = <&cmu_hp SIWX91X_CLK_UART0>;``.
