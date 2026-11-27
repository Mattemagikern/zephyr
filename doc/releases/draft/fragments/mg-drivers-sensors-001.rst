::area:: Drivers: Sensors
::pr:: 117953

::migration-guide::
The :dtcompatible:`pixart,paa3905` driver now enforces the sensor's
datasheet SPI contract: mode 3 is set by the driver and a devicetree
``spi-max-frequency`` above 2 MHz fails the build. Out-of-tree boards
that overclocked the bus must lower the property to 2000000 or less.
