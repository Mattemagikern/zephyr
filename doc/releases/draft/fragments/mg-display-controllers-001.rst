::area:: Display controllers
::pr:: 104099

::migration-guide::
The Kconfig options ``CONFIG_SDL_DISPLAY_DEFAULT_PIXEL_FORMAT_*`` for SDL display pixel-format
selection have been removed in favour of setting the pixel-format property directly in devicetree
on the SDL pseudo-device node using the PANEL_PIXEL_FORMAT_* macros from
:zephyr_file:`include/zephyr/dt-bindings/display/panel.h`.
