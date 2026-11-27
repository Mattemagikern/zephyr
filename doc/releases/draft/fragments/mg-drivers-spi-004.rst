::area:: Drivers: SPI
::pr:: 116713

::migration-guide::
The optional delay argument of :c:macro:`SPI_CONFIG_DT`, :c:macro:`SPI_CONFIG_DT_INST`,
:c:macro:`SPI_DT_SPEC_GET`, :c:macro:`SPI_DT_SPEC_INST_GET`, :c:macro:`SPI_DT_IODEV_DEFINE`,
:c:macro:`SPI_DT_INST_IODEV_DEFINE` and :c:macro:`SPI_CS_CONTROL_INIT` has been removed; drop
it from every invocation. Use the ``spi-cs-setup-delay-ns`` and ``spi-cs-hold-delay-ns``
devicetree properties instead (note that they are in nanoseconds rather than microseconds).
