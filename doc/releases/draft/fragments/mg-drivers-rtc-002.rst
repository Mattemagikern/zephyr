::area:: Drivers: RTC
::pr:: 108772

::migration-guide::
:dtcompatible:`microcrystal,rv3032` properties ``trickle-resistor-ohms`` and
``trickle-charger-mode`` have moved to the parent
:dtcompatible:`microcrystal,rv3032-mfd` device. The parent MFD device now
handles configuring the backup supply mode for all child devices.
