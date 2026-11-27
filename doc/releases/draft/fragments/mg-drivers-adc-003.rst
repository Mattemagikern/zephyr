::area:: Drivers: ADC
::pr:: 116995

::migration-guide::
The ``CONFIG_LPADC_CHANNEL_COUNT`` Kconfig option has been removed. The NXP LPADC driver now
treats hardware command slots as logical ADC channels and derives the number of logical channels
per instance from the ``channel`` child nodes declared for that instance in devicetree, so unused
command slots no longer consume RAM. Applications that lowered the Kconfig option to save RAM
should simply drop it. An instance that declares no ``channel`` node keeps the full hardware
capacity available, so applications that only ever configure channels at runtime through
:c:func:`adc_channel_setup` are unaffected; applications that mix both must declare in
devicetree the highest channel identifier they set up at runtime. Declaring a channel identifier
beyond the number of ``CMD`` registers implemented by the SoC is now a build error instead of a
runtime HAL assertion, and :c:func:`adc_read` now rejects an empty channel mask, or one selecting
channels beyond that limit, with ``-EINVAL`` instead of silently ignoring it.
