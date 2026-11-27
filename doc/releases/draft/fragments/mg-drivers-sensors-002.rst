::area:: Drivers: Sensors
::pr:: 104808

::migration-guide::
The ``girqs`` and ``pcrs`` properties (array type) of :dtcompatible:`microchip,xec-tach` have been
replaced by ``pcr-scr`` (int type) to use encoded PCR register index and bit position macros.
GIRQ configuration is now handled via the ``microchip,dmec-ecia-girq`` binding include.
