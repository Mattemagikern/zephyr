::area:: STM32 Platforms
::pr:: 117309

::migration-guide::
:dtcompatible:`st,stm32-adc` binding has been restructured to reflect the ADC hardware
topology. A parent node now represents the ADC common block, which holds the clock and
the settings shared by all the ADC instances connected to it, while a new ``child-binding``
represents the ADC instances themselves.

The existing ``&adcN`` node labels still designate the ADC instances, which are now children
of a common block node labelled ``&adcN_common``, where ``N`` lists the instances sharing the
block (for example ``&adc1_common``, ``&adc12_common`` or ``&adc123_common``). The common block
node must be enabled in addition to the instance node.

The following properties shall be moved from the ``&adcN`` instance node to its ``&adcN_common``
parent node: ``clocks``, ``clock-names``, ``st,adc-clock-source``, ``st,adc-prescaler`` and
``vref-mv``. Since the clock is now described once per common block, instances sharing it can
no longer be given conflicting clock settings.

.. tabs::

  .. group-tab:: Before

    .. code-block:: devicetree

        &adc1 {
          clocks = <&rcc STM32_CLOCK(AHB2, 13)>,
                   <&rcc STM32_SRC_SYSCLK ADC_SEL(3)>;
          clock-names = "adcx", "adc_ker";
          st,adc-clock-source = "ASYNC";
          st,adc-prescaler = <4>;
          vref-mv = <3000>;
          pinctrl-0 = <&adc1_in1_pa0>;
          pinctrl-names = "default";
          status = "okay";
        };

  .. group-tab:: After

    .. code-block:: devicetree

        &adc12_common {
          clocks = <&rcc STM32_CLOCK(AHB2, 13)>,
                   <&rcc STM32_SRC_SYSCLK ADC_SEL(3)>;
          clock-names = "adcx", "adc_ker";
          st,adc-clock-source = "ASYNC";
          st,adc-prescaler = <4>;
          vref-mv = <3000>;
          status = "okay";
        };

        &adc1 {
          pinctrl-0 = <&adc1_in1_pa0>;
          pinctrl-names = "default";
          status = "okay";
        };

Note that ``vref-mv`` only needs to be set when it differs from its ``3300`` default value.

For :dtcompatible:`st,stm32f1-adc` and :dtcompatible:`st,stm32f4-adc`, each instance keeps its
own register clock, so ``clocks`` and ``clock-names`` stay on the ``&adcN`` node.
