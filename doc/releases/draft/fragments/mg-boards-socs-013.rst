::area:: Boards/SoCs
::pr:: 109470

::migration-guide::
The system clock on STM32H7RS-based boards (stm32h7s78_dk and nucleo_h7s3l8)
has been increased to 600 MHz. This is achieved by increasing the PLL1 frequency
to 300 MHz, which also affects the bus and kernel clocks, resulting in slightly
higher frequencies.
