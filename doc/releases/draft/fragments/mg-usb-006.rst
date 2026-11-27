::area:: USB
::pr:: 112477

::migration-guide::
The ``clock-names`` property is now required on :dtcompatible:`st,stm32-usbphyc` nodes.
A default value is provided at SoC DTSI level but *might* need to be overridden by board DTS.
