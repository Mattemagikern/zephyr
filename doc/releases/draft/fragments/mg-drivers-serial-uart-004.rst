::area:: Drivers: Serial/UART
::pr:: 120368

::migration-guide::
:kconfig:option:`CONFIG_UART_NS16550_DW8250_DW_APB` now follows devicetree: it is
enabled for :dtcompatible:`snps,dw-apb-uart` nodes and cannot be set otherwise. Add that
compatible to the UART nodes instead of setting the option in Kconfig.
