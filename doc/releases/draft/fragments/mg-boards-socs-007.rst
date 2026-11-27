::area:: Boards/SoCs
::pr:: 122679

::migration-guide::
The internal Nordic SoC platform Kconfig symbols ``CONFIG_NRF_PLATFORM_HALTIUM``
and ``CONFIG_NRF_PLATFORM_LUMOS`` are no longer used by in-tree code, which now
relies on explicit :kconfig:option:`CONFIG_SOC_SERIES_NRF54H`,
:kconfig:option:`CONFIG_SOC_SERIES_NRF92`,
:kconfig:option:`CONFIG_SOC_SERIES_NRF54L` and
:kconfig:option:`CONFIG_SOC_SERIES_NRF71` checks. Both symbols are kept
as deprecated stubs that default to ``y`` when the corresponding SoC
series is selected and :kconfig:option:`CONFIG_NRF_PLATFORM_DEPRECATED_SYMBOLS` is enabled,
so existing ``CONFIG_NRF_PLATFORM_*=y`` lines and ``depends on NRF_PLATFORM_*`` clauses keep
building with a Kconfig deprecation warning. Out-of-tree Kconfig, CMake and code using these
symbols should be updated:

* ``CONFIG_NRF_PLATFORM_HALTIUM`` with
  :kconfig:option:`CONFIG_SOC_SERIES_NRF54H` or
  :kconfig:option:`CONFIG_SOC_SERIES_NRF92`.
* ``CONFIG_NRF_PLATFORM_LUMOS`` with
  :kconfig:option:`CONFIG_SOC_SERIES_NRF54L` or
  :kconfig:option:`CONFIG_SOC_SERIES_NRF71`.
