::area:: Drivers: I2C
::pr:: 118869

::migration-guide::
The :dtcompatible:`nxp,sc18im704-i2c` bridge no longer sends the target address
unshifted to the SC18IM704. The Zephyr I2C API passes a 7-bit address to a controller's
``transfer()`` callback, and the driver now shifts it left by one to build the address byte
the bridge expects. Devicetree nodes sitting on a :dtcompatible:`nxp,sc18im704-i2c` bus
that compensated for the missing shift by declaring a pre-shifted ``reg`` (for example
``reg = <0xa0>`` for a device at address ``0x50``) must now declare the real 7-bit address
(``reg = <0x50>``).
