::area:: Boards/SoCs
::pr:: 116741

::migration-guide::
The Nordic nRF52 Kconfig option ``CONFIG_GPIO_AS_PINRESET`` has been removed. Set the
``gpio-as-nreset`` property on the ``&uicr`` devicetree node instead.

The Nordic Kconfig options ``CONFIG_SOC_DCDC_NRF52X``, ``CONFIG_SOC_DCDC_NRF52X_HV``,
``CONFIG_SOC_DCDC_NRF53X_APP``, ``CONFIG_SOC_DCDC_NRF53X_NET`` and
``CONFIG_SOC_DCDC_NRF53X_HV`` have been removed. Configure the regulators in devicetree
instead: ``regulator-initial-mode = <NRF5X_REG_MODE_DCDC>`` on ``&reg1``/``&vregmain``/
``&vregradio``, and ``status = "okay"`` on ``&reg0``/``&vregh``.

The Nordic nRF53 Kconfig option ``CONFIG_BOARD_ENABLE_CPUNET`` has been removed. Use
:kconfig:option:`CONFIG_SOC_NRF53_CPUNET_ENABLE` instead.
