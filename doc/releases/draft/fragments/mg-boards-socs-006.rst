::area:: Boards/SoCs
::pr:: 107874

::migration-guide::
The Kconfig options :kconfig:option:`CONFIG_SRAM_SIZE` and
:kconfig:option:`CONFIG_SRAM_BASE_ADDRESS` have been deprecated, boards should instead use the
devicetree ``zephyr.sram`` chosen node to specify the RAM node which will be used (whose values
populated the Kconfig values). If either option is manually adjusted, it will cause
:kconfig:option:`CONFIG_SRAM_DEPRECATED_KCONFIG_SET` to be set which indicates this deprecation.
