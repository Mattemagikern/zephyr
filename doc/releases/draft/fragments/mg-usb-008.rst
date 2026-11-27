::area:: USB
::pr:: 117882

::migration-guide::
The ``clock-reference`` property of :dtcompatible:`st,stm32u5-otghs-phy` is now deprecated
and should be removed from DTS files; the underlying driver will compute the correct value
automatically if the property doesn't exist (and honor it otherwise).
