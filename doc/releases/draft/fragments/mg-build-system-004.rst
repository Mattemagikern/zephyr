::area:: Build system
::pr:: 115298

::migration-guide::
CMake variables ``SOC_NAME``, ``SOC_SERIES``, ``SOC_FAMILY`` and ``SOC_V2_DIR`` have been
deprecated as they duplicate variables already, the replacement variables are as follows:
:kconfig:option:`CONFIG_SOC`, :kconfig:option:`CONFIG_SOC_SERIES`,
:kconfig:option:`CONFIG_SOC_FAMILY` and ``SOC_FULL_DIR``.
