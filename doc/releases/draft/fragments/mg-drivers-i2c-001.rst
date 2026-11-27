::area:: Drivers: I2C
::pr:: 114186

::migration-guide::
On controller based on :kconfig:option:`CONFIG_I2C_DW` the
``CONFIG_I2C_DW_RW_TIMEOUT_MS`` option has been replaced with
:kconfig:option:`CONFIG_I2C_TRANSFER_TIMEOUT_MS`, with a default of 500ms.
