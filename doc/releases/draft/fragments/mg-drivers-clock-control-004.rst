::area:: Drivers: Clock control
::pr:: 117823

::migration-guide::
.. toggle::

   The :ref:`clock_control_api` driver has been updated for the following clocks on nRF52, nRF53, nRF91, and nRF54L Series devices:

   * HFCLK
   * LFCLK
   * XO
   * XO24M
   * HFCLK192M
   * HFCLKAUDIO

   To restore the legacy driver implementation, set :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF` to ``y``.

   To migrate your code from zephyr v4.4.0 to zephyr v4.5.0, complete the following steps:

   1. Enable each application-controlled clock in the application-specific or board-specific devicetree overlay file.

      This enables the corresponding clock driver.
      For example:

      .. code-block:: dts

          /* if nRF54L XO is to be controlled */
          &xo {
              status = "okay";
          };

          /* if nRF52, nRF53 HFCLK is to be controlled */
          &hfclk {
              status = "okay";
          };

          /* if nRF52, nRF53, nRF91 or nRF54L LFCLK is to be controlled */
          &lfclk {
              status = "okay";
          };

          /* if HFCLK192M is to be controlled */
          &hfclk192m {
              status = "okay";
          };

          /* if XO24M is to be controlled */
          &xo24m {
              status = "okay";
          };

          /* if HFCLKAUDIO is to be controlled */
          &hfclkaudio {
              status = "okay";
          };

   #. Rename the following Kconfig options:

      * Replace :kconfig:option:`CONFIG_NRFX_CLOCK_LF_CAL_ENABLED` with :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF_K32SRC_RC_CALIBRATION`.

   #. Move the following Kconfig options to the ``nordic,nrf-clock-lfclk`` devicetree node:

      * Replace :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF_K32SRC_FREQUENCY` with the ``k32src-frequency`` property.
      * Replace :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF_SOURCE` choice with the ``k32src`` enum property.
      * Replace :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF_K32SRC_RC` and :kconfig:option:`CONFIG_NRFX_CLOCK_LF_SRC_RC` with the ``k32src = "rc"``.
      * Replace :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF_K32SRC_XTAL` and :kconfig:option:`CONFIG_NRFX_CLOCK_LF_SRC_XTAL` with the ``k32src = "xtal"``.
      * Replace :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF_K32SRC_SYNTH` and :kconfig:option:`CONFIG_NRFX_CLOCK_LF_SRC_SYNTH` with the ``k32src = "synth"``.
      * Replace :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF_K32SRC_EXT_LOW_SWING` and :kconfig:option:`CONFIG_NRFX_CLOCK_LF_SRC_LOW_SWING` with the ``k32src = "ext_low_swing"``.
      * Replace :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF_K32SRC_EXT_FULL_SWING` and :kconfig:option:`CONFIG_NRFX_CLOCK_LF_SRC_FULL_SWING` with the ``k32src = "ext_full_swing"``.
      * Replace :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF_ACCURACY_PPM` choice with the ``k32src-accuracy-ppm`` enum property.
      * Replace :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF_K32SRC_500PPM` with the ``k32src-accuracy-ppm = <500>``.
      * Replace :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF_K32SRC_250PPM` with the ``k32src-accuracy-ppm = <250>``.
      * Replace :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF_K32SRC_150PPM` with the ``k32src-accuracy-ppm = <150>``.
      * Replace :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF_K32SRC_100PPM` with the ``k32src-accuracy-ppm = <100>``.
      * Replace :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF_K32SRC_75PPM` with the ``k32src-accuracy-ppm = <75>``.
      * Replace :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF_K32SRC_50PPM` with the ``k32src-accuracy-ppm = <50>``.
      * Replace :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF_K32SRC_30PPM` with the ``k32src-accuracy-ppm = <30>``.
      * Replace :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF_K32SRC_20PPM` with the ``k32src-accuracy-ppm = <20>``.
      * Replace :kconfig:option:`CONFIG_NRFX_CLOCK_LFXO_TWO_STAGE_ENABLED` with ``k32src = "xtal"`` or ``k32src = "ext_low_swing"`` or ``k32src = "ext_full_swing"``.

   #. Update your application to use the new clock control API.

      Use the following mapping when you update the API calls:

      * ``mgr`` is the on-off manager created for ``nordic,nrf-clock`` and obtained using ``z_nrf_clock_control_get_onoff``.
      * ``dev`` is the device compatible with ``nordic,nrf-clock``.
      * ``sys`` is the subsystem for ``nordic,nrf-clock``.
         The new clocks implementation does not use it.
      * ``new_dev`` is the device that corresponds to the previously used ``sys`` value.
        It must be compatible with one of the following nodes:

        * ``nordic,nrf-clock-lfclk``
        * ``nordic,nrf-clock-hfclk``
        * ``nordic,nrf-clock-xo``
        * ``nordic,nrf-clock-hfclk192m``
        * ``nordic,nrf-clock-xo24m``
        * ``nordic,nrf-clock-hfclkaudio``

      The following example shows the deprecated API usage and the corresponding new API usage:

      .. code-block:: c

         // Old API usage (deprecated)
         z_nrf_clock_calibration_init(&mgrs);    //1
         onoff_release(mgr)                      //2
         onoff_request(mgr, &cli);               //3
         onoff_cancel_or_release(mgr, &cli);     //4
         clock_control_on(dev,sys)               //5
         clock_control_off(dev,sys)              //6
         clock_control_async_on(dev,sys)         //7
         clock_control_get_status(dev,sys)       //8
         z_nrf_clock_control_get_onoff(sys)      //9

         // New API usage
         z_nrf_clock_calibration_init();                             //1
         nrf_clock_control_release(new_dev, NULL);                   //2
         nrf_clock_control_request(new_dev, NULL, &cli);             //3
         nrf_clock_control_cancel_or_release(new_dev, NULL, &cli);   //4
         clock_control_on(new_dev, NULL)                             //5
         clock_control_off(new_dev, NULL)                            //6
         clock_control_async_on(new_dev, NULL)                       //7
         clock_control_get_status(new_dev, NULL)                     //8
         // Remove all uses of z_nrf_clock_control_get_onoff         //9
