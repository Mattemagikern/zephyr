::area:: Drivers: Serial/UART
::pr:: 121456

::migration-guide::
The ``ambiq,pl011-uart`` devicetree binding and the Ambiq support in the PL011 serial driver have
been removed. Apollo2, Apollo3 and Apollo3 Blue Plus UART nodes now use the
:dtcompatible:`ambiq,uart` compatible, which is handled by the Ambiq UART driver. Out-of-tree
boards and SoCs that use ``ambiq,pl011-uart`` must switch to :dtcompatible:`ambiq,uart`.
