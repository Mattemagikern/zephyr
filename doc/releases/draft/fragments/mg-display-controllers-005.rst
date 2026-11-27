::area:: Display controllers
::pr:: 112528

::migration-guide::
The ``set_contrast`` function of the ST7567 display driver now expects a contrast value in the
range 0-255, instead of 0-63. The driver now scales the value to the 6-bit range expected by the
controller. The ``CONFIG_ST7567_DEFAULT_CONTRAST`` Kconfig option has been updated to reflect the
new range.
