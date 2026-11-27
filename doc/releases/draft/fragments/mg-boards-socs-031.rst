::area:: Boards/SoCs
::pr:: 117182

::migration-guide::
The Espressif per-module devicetree include files and their SoC Kconfig symbols have been
removed. A module or SIP part number describes how much flash and PSRAM a board carries, which
is a property of the board rather than of the SoC, so both are now declared by the board itself.

Every ``espressif/<soc>/<soc>_<module>.dtsi`` file is replaced by a single
``espressif/<soc>/<soc>.dtsi`` per SoC. The matching hidden Kconfig symbols, such as
``SOC_ESP32S3_WROOM_N8`` and ``SOC_ESP32_WROVER_E_N16R8``, are replaced by the plain SoC symbol,
such as :kconfig:option:`CONFIG_SOC_ESP32S3`. ``SOC_PART_NUMBER`` now reports the SoC rather than
the module.

Out-of-tree Espressif boards must be updated, and fail to build until they are:

* Include the plain SoC dtsi instead of the module one.
* Select the plain SoC symbol in ``Kconfig.<board>``.
* Describe the flash in the board dts, giving both ``reg`` and a matching ``ranges``, because
  the SoC dtsi no longer sets either.

  .. code-block:: devicetree

     &flash0 {
         reg = <0x0 DT_SIZE_M(8)>;
         ranges = <0x0 0x0 DT_SIZE_M(8)>;
     };

* Describe the PSRAM the same way, on boards that have it:

  .. code-block:: devicetree

     &psram0 {
         size = <DT_SIZE_M(2)>;
     };

On the dual-core ESP32, ``espressif/esp32/esp32_appcpu.dtsi`` no longer sets a flash either, so
an APPCPU board dts has to declare the same flash as its PROCPU counterpart.
