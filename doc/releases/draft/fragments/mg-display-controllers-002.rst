::area:: Display controllers
::pr:: 110533

::migration-guide::
The LVGL ``CONFIG_LV_Z_COLOR_24_BGR_TO_RGB`` Kconfig option has been removed. LVGL's RGB888 color
format stores bytes in memory as blue, green, red, which matches the in-memory layout of
:c:enumerator:`PIXEL_FORMAT_RGB_888`, so no channel swap is performed for displays reporting that
format. Displays whose framebuffer instead expects a red, green, blue byte order must now report
:c:enumerator:`PIXEL_FORMAT_BGR_888`, for which the LVGL glue performs the red/blue channel swap
automatically.
