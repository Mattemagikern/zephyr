::area:: Drivers: Flash
::pr:: 107502

::migration-guide::
:dtcompatible:`jedec,spi-nand` now requires a ``plane-bytes`` property, which indicates the size
of each plane in the flash device. For devices with a single plane, this should be set to the
same value as ``size-bytes``.
