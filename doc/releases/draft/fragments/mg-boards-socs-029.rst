::area:: Boards/SoCs
::pr:: 118151

::migration-guide::
The STM32MP15 Cortex-M4 SoC Kconfig symbol ``SOC_STM32MP15_M4`` has been renamed to
:kconfig:option:`CONFIG_SOC_STM32MP157CXX_M4`. Out-of-tree STM32MP15 boards that selected
``SOC_STM32MP15_M4`` must select :kconfig:option:`CONFIG_SOC_STM32MP157CXX_M4` instead.
