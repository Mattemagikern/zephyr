::area:: Drivers: ADC
::pr:: 117114

::release-notes::
STM32 ADC driver (:dtcompatible:`st,stm32-adc`): when
:kconfig:option:`CONFIG_ADC_STM32_VREFINT_CALIBRATE` is enabled,
:c:func:`adc_ref_internal` may return a measured scale instead of DT
``vref-mv``. Any ADC named by an :dtcompatible:`st,stm32-vref`
``io-channels`` property can take that measurement; the result is
cached SoC-wide. See the :ref:`migration guide<migration_4.5>` ADC section.

STM32G4 SoC dtsi files now describe the extra VREFINT inputs that exist in
silicon: :dtcompatible:`st,stm32-vref` ``vref3`` (ADC3, G491 and up),
``vref4`` and ``vref5`` (ADC4/ADC5, G473 and up). Nodes stay disabled;
boards enable the instance they use. ADC2 has no VREFINT mux.
