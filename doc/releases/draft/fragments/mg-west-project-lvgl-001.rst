::area:: West project: lvgl
::pr:: 120838

::migration-guide::
LVGL was updated from v9.5 to v9.6. See the `LVGL migration guide
<https://lvgl.io/docs/open/changelog/migration-v9-6>`_ for the changes of LVGL itself.

LVGL v9.6 deprecates the ``LV_COLOR_DEPTH`` choice in favor of the ``LV_COLOR_FORMAT_DEFAULT``
choice, and ``CONFIG_LV_COLOR_DEPTH_*`` options no longer select the color format in Zephyr.
Replace them with ``CONFIG_LV_COLOR_FORMAT_*``, for example ``CONFIG_LV_COLOR_DEPTH_16=y`` with
``CONFIG_LV_COLOR_FORMAT_RGB565=y``.

``CONFIG_LV_ATTRIBUTE_MEM_ALIGN_SIZE`` was replaced by
:kconfig:option:`CONFIG_LV_Z_ATTRIBUTE_MEM_ALIGN_SIZE`.

Boards, shields and SoCs no longer set LVGL options in ``Kconfig.defconfig`` files, as these
options are not defined when the LVGL module is absent. They set Zephyr options instead, which
provide the defaults of the LVGL options. Out-of-tree boards, shields and SoCs have to replace:

* the ``LV_COLOR_DEPTH`` choice with the ``LV_Z_COLOR_FORMAT`` choice, for example
  ``LV_COLOR_DEPTH_16`` with ``LV_Z_COLOR_FORMAT_RGB565``,
* ``LV_DPI_DEF`` and ``LV_DRAW_BUF_ALIGN`` with :kconfig:option:`CONFIG_LV_Z_DPI_DEF` and
  :kconfig:option:`CONFIG_LV_Z_DRAW_BUF_ALIGN`,
* ``LV_USE_DRAW_DMA2D`` with :kconfig:option:`CONFIG_LV_Z_DRAW_DMA2D_DEFAULT`,
* ``LV_USE_DRAW_DMA2D_INTERRUPT`` and ``LV_DRAW_DMA2D_HAL_INCLUDE`` with
  :kconfig:option:`CONFIG_LV_Z_DRAW_DMA2D_INTERRUPT` and
  :kconfig:option:`CONFIG_LV_Z_DRAW_DMA2D_HAL_INCLUDE`,
* ``LV_NEMA_USE_CACHE`` and ``LV_NEMA_CACHE_HAL_INCLUDE`` with
  :kconfig:option:`CONFIG_LV_Z_NEMA_USE_CACHE` and
  :kconfig:option:`CONFIG_LV_Z_NEMA_CACHE_HAL_INCLUDE`,
* the ``LV_USE_NEMA_LIB`` choice with one of the ``LV_Z_NEMA_LIB_*`` options, for example
  :kconfig:option:`CONFIG_LV_Z_NEMA_LIB_M7`.

The ``LV_USE_NEMA_HAL`` choice needs no replacement, as the custom HAL used by Zephyr is the LVGL
default. Applications setting LVGL options in ``.conf`` files are not affected.

The log level of the Zephyr LVGL integration no longer follows the LVGL log level. It is now
selected with the standard :kconfig:option:`CONFIG_LV_Z_LOG_LEVEL` choice
(``CONFIG_LV_Z_LOG_LEVEL_DBG`` etc.) and defaults to :kconfig:option:`CONFIG_LOG_DEFAULT_LEVEL`.
LVGL's own logging is still configured with ``CONFIG_LV_USE_LOG`` and ``CONFIG_LV_LOG_LEVEL_*``.
LVGL messages forwarded to the Zephyr logging subsystem are filtered by both levels, so set
``CONFIG_LV_Z_LOG_LEVEL_DBG=y`` to see LVGL trace messages.

``CONFIG_LV_USE_SYSMON`` no longer implies :kconfig:option:`CONFIG_CPU_LOAD`. Enable
:kconfig:option:`CONFIG_CPU_LOAD` for the system monitor to report the CPU usage.
