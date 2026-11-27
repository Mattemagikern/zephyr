::area:: Drivers: I2C
::pr:: 114182

::migration-guide::
The ITE I2C controllers :dtcompatible:`ite,enhance-i2c`
:dtcompatible:`ite,it51xxx-i2c` :dtcompatible:`ite,it8xxx2-i2c` transfer
timeout is now using the generic ``zephyr,transfer-timeout-ms`` property
instead of ``transfer-timeout-ms``, default to 500ms.
