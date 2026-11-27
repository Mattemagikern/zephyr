::area:: Display controllers
::pr:: 117765

::migration-guide::
The ``chipone,co5300`` MIPI DSI display driver no longer maintains an
internal shadow framebuffer, and the ``pitch-align``, ``addr-align``, and
``ext-ram`` devicetree properties have been removed from the
:dtcompatible:`chipone,co5300` binding. Boards previously relying on these
properties to satisfy display-controller alignment requirements should
instead enable :kconfig:option:`CONFIG_LV_Z_AREA_X_ALIGNMENT_WIDTH` and
:kconfig:option:`CONFIG_LV_Z_AREA_Y_ALIGNMENT_WIDTH` (LVGL) so that
invalidated areas are rounded to the required boundary before reaching the
driver.
