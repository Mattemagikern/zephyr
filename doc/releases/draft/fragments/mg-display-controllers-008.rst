::area:: Display controllers
::pr:: 120565

::migration-guide::
The ssd16xx display driver now reports :c:enumerator:`PIXEL_FORMAT_MONO01` instead of
:c:enumerator:`PIXEL_FORMAT_MONO10`, which matches the controller RAM, where a set bit is a
white pixel. LVGL and the character framebuffer draw with the intended colors without any
change. Applications that pass their own buffers to :c:func:`display_write` and inverted them
to compensate for the former pixel format must drop that inversion.
