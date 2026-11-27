::area:: Display controllers
::pr:: 108610

::migration-guide::
LVGL now renders directly in its ``RGB565_SWAPPED`` color format for displays reporting
:c:enumerator:`PIXEL_FORMAT_RGB_565X`, so :kconfig:option:`CONFIG_LV_COLOR_16_SWAP` is no longer
needed for these displays and must not be enabled together with that pixel format, otherwise the
buffer ends up byte-swapped twice. The ``t_deck``, ``m5stack_core2`` and ``wio_terminal`` boards
no longer enable the option by default.
