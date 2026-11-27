::area:: Drivers: ADC
::pr:: 110353

::migration-guide::
The :kconfig:option:`CONFIG_LPADC_DO_OFFSET_CALIBRATION` option is now only meaningful when
:kconfig:option:`CONFIG_ADC_MCUX_LPADC` is enabled, and its ``default y`` is now scoped to that
condition. In-tree boards no longer enable it explicitly in their defconfigs since
the default already covers them.
