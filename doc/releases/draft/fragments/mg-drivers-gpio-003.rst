::area:: Drivers: GPIO
::pr:: 115299

::migration-guide::
The :dtcompatible:`awinic,aw9523b-gpio` driver no longer has the ``reset-gpios`` property. This has instead
been moved to the parent :dtcompatible:`awinic,aw9523b` MFD device.
