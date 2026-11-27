::area:: Drivers: Sensors
::pr:: 110123

::migration-guide::
The devicetree compatible ``tdk,ntcg163jf103ft1`` has been renamed to
:dtcompatible:`tdk,ntcgxx3jx103x` to reflect that the compensation values are identical for TDK
NTCG thermistor parts with the same resistance (R25) and beta (B25/85) values, as indicated in the
part naming scheme.
