::area:: Boards/SoCs
::pr:: 107944

::migration-guide::
Boards that use UF2 images that migrate to :dtcompatible:`zephyr,mapped-partition` should enable
HEX output in their defconfig (:kconfig:option:`CONFIG_BUILD_OUTPUT_HEX`), as the UF2 image
generation can no longer rely on :kconfig:option:`CONFIG_FLASH_LOAD_OFFSET` to determine the code
address from a BIN output. HEX to UF2 is now the default (instead of BIN).
