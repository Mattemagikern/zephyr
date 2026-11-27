::area:: Drivers: Counter
::pr:: 117709

::migration-guide::
:dtcompatible:`adi,max32-rtc-counter` and :dtcompatible:`adi,max32-wut` now use the shared
``clk_32k`` node for 32 kHz clock source selection. The clock source is now configured through the
``clocks`` property of the ``clk_32k`` node, instead of ``clock-source`` property in each
peripheral node.
