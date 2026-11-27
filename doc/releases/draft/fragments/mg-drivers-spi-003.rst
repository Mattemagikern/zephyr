::area:: Drivers: SPI
::pr:: 110265

::migration-guide::
The ``fifo-enable`` property of :dtcompatible:`st,stm32h7-spi` has been removed.
FIFO is now always used in polling and interrupt mode to enhance performance. A new property
``st,fifo-threshold`` can be used to configure the FIFO threshold (default = 1).
