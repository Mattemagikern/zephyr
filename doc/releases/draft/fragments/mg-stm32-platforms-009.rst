::area:: STM32 Platforms
::pr:: 109903

::migration-guide::
:dtcompatible:`st,stm32-xspim` is now also used on STM32H5 and STM32H7RS series
to declare and configure XSPI Manager. Boards making use of XSPI must now enable
``&xspim`` node in addition to the desired XSPI controller to use XSPI.
