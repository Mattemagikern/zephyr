::area:: Drivers: PWM
::pr:: 104570

::migration-guide::
The ``pcrs`` property (array type) of :dtcompatible:`microchip,xec-pwm` has been replaced
by ``pcr-scr`` (int type) to use encoded PCR register index and bit position macros.
