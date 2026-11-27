::area:: Boards/SoCs
::pr:: 116956

::migration-guide::
The ``esp_threadbr_ethernet`` shield has been removed. Existing users should
build for ``esp_threadbr/esp32s3/procpu/ethernet`` instead of combining
``esp_threadbr/esp32s3/procpu`` with ``SHIELD=esp_threadbr_ethernet``.
Along with the shield, the ``esp_threadbr`` sub-board connector description
has been removed, so the ``espressif,esp-threadbr-header`` binding, the
``esp_threadbr_header`` GPIO nexus node and the ``esp_threadbr_spi`` and
``esp_threadbr_i2c`` devicetree labels are gone. Out-of-tree overlays using
them have to reference the SoC nodes (``&spi2``, ``&i2c0``, ``&gpio0``,
``&gpio1``) directly.
