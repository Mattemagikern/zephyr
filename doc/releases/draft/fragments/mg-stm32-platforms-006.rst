::area:: STM32 Platforms
::pr:: 117114

::migration-guide::
STM32 ADC (:dtcompatible:`st,stm32-adc`): when
:kconfig:option:`CONFIG_ADC_STM32_VREFINT_CALIBRATE` is enabled (default
whenever an okay :dtcompatible:`st,stm32-vref` node exists),
:c:func:`adc_ref_internal` and INTERNAL :c:func:`adc_raw_to_millivolts_dt`
results may no longer match DT ``vref-mv`` / 3300 exactly. Disable the
Kconfig to keep the previous static DT-only scale.

``adc_ref_internal()`` no longer reports the first common block's
``vref-mv`` for every STM32 ADC child. Each child uses its parent common
block's ``vref-mv`` (optional, default 3300). This only affects multi-common
DTs that set divergent ``vref-mv`` values and relied on the old shared
``DEVICE_API``.

VREFINT measurement is no longer tied to the first okay
:dtcompatible:`st,stm32-vref` node. Any enabled ADC child named by a vref
node's ``io-channels`` (including disabled vref nodes) can refresh the
shared rail cache.
