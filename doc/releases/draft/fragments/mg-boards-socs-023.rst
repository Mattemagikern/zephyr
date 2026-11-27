::area:: Boards/SoCs
::pr:: 116218

::migration-guide::
:kconfig:option:`CONFIG_SPI_STM32_INTERRUPT` default activation is removed from STM32 based boards
that used to do it.
Choosing interrupt-driven vs polling SPI transfers is an application concern, not a board one.
Applications that rely on interrupt-driven SPI (for example to use :c:func:`spi_transceive_signal`
or :c:func:`spi_transceive_cb` without DMA) on an affected board must now explicitly enable
:kconfig:option:`CONFIG_SPI_STM32_INTERRUPT` in their own configuration.
