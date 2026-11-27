::area:: Drivers: Counter
::pr:: 116789

::migration-guide::
The ``prescaler`` property of :dtcompatible:`nxp,lptmr` has been removed. Use
``prescale-glitch-filter`` and ``prescale-glitch-filter-bypass`` instead. The new property is
an exponent, not a divisor: the prescaler divides by ``2^(prescale-glitch-filter + 1)``.
