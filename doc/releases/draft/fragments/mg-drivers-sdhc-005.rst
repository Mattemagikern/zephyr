::area:: Drivers: SDHC
::pr:: 113608

::migration-guide::
:kconfig:option:`CONFIG_SDMMC_STM32_HWFC` is now enabled by default for the legacy SDMMC_STM32
disk driver to prevent FIFO underrun and overrun errors during disk access. Applications that
previously set ``CONFIG_SDMMC_STM32_HWFC=y`` should remove this configuration from their board
configuration files since it is now the default.
