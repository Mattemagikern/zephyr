::area:: Drivers: Haptics
::pr:: 118494

::migration-guide::
The ``vib-rated-mv`` and ``vib-overdrive-mv`` properties of :dtcompatible:`ti,drv2605` now
default to the device reset values, 1362 mV and 3075 mV, instead of 3200 mV. Boards that need
the previous drive level must set them explicitly.
