::area:: Drivers: ADC
::pr:: 105658

::migration-guide::
The ``girqs`` and ``pcrs`` properties (array type) of :dtcompatible:`microchip,xec-adc` have been
replaced by encoded ``girqs`` (using ``MCHP_XEC_ECIA_GIRQ_ENC`` macros) and ``pcr-scr`` (int type)
for encoded PCR register index and bit position.
