::area:: Drivers: SPI
::pr:: 116155

::migration-guide::
The SPI API has been migrated to the inclusive terminology selected by
:ref:`coding_guideline_inclusive_language` (controller/peripheral roles, SDO/SDI signal names).
The former names are deprecated and will be removed in Zephyr v5.0:

* :c:macro:`SPI_OP_MODE_CONTROLLER` and :c:macro:`SPI_OP_MODE_PERIPHERAL` replace
  ``SPI_OP_MODE_MASTER`` and ``SPI_OP_MODE_SLAVE``.
* The ``slave`` member of :c:struct:`spi_config` is renamed to ``peripheral``.
* :c:macro:`SPI_SDO_OVERRUN_UNKNOWN`, :c:macro:`SPI_SDO_OVERRUN_DT` and
  :c:macro:`SPI_SDO_OVERRUN_DT_INST` replace the ``SPI_MOSI_OVERRUN_*`` macros.
* :kconfig:option:`CONFIG_SPI_PERIPHERAL` replaces ``CONFIG_SPI_SLAVE``.
* ``spi_context_is_peripheral()`` replaces ``spi_context_is_slave()`` in the driver-facing
  ``spi_context.h`` helpers.
* The ``sdo-gpios``/``sdi-gpios`` properties of :dtcompatible:`zephyr,spi-bitbang` and
  :dtcompatible:`raspberrypi,pico-spi-pio`, the ``spi-sdi-gpios``/``spi-sdo-gpios`` properties of
  :dtcompatible:`brcm,afbr-s50`, the ``peripheral`` property of :dtcompatible:`nxp,s32-spi` and
  the ``is-peripheral`` property of :dtcompatible:`realtek,bee-spi` replace their former
  ``mosi``/``miso``/``slave``-based names, which are still accepted but marked as deprecated in
  the bindings.
* The :dtcompatible:`zephyr,bt-hci-spi-peripheral` devicetree compatible replaces
  ``zephyr,bt-hci-spi-slave``.
