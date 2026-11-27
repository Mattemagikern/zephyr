::area:: Boards/SoCs
::pr:: 109468

::migration-guide::
:kconfig:option:`CONFIG_GPIO` is no longer enabled by default on most STM32 boards.
(boards with GPIO hogs keep it enabled as GPIO is needed for hogs to work).
Applications that relied on ``CONFIG_GPIO=y`` being the default will need to enable
the option explicitly.
