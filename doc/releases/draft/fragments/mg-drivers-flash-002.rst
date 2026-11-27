::area:: Drivers: Flash
::pr:: 114971

::migration-guide::
The :dtcompatible:`st,stm32-nv-flash` property ``bank2-flash-size`` has been deprecated in favor
of determining flash bank sizes using ``reg`` size cells. No changes need be made to the
devicetree save for removing the aforementioned property.
