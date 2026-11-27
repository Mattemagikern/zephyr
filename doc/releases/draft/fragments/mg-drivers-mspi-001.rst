::area:: Drivers: MSPI
::pr:: 122360

::migration-guide::
STM32 external serial memory support is migrating from the legacy QSPI, OSPI, and XSPI
flash and RAM drivers to separate MSPI controller and memory-device drivers. The new drivers
provide a more flexible model that supports multiple memory devices on a single controller
and a unified interface with clear layering for all external serial memories,
including NOR flashes and external RAMs.

The migration is ongoing in Zephyr 4.5 and is planned for completion in the next release.
The legacy STM32 controller-specific external flash and RAM driver model will be officially
deprecated in favor of MSPI in that release.

Application-facing APIs remain unchanged. The migration changes the devicetree bindings and
Kconfig options to use the MSPI model only on impacted boards. SoC devicetree descriptions
are unchanged in Zephyr 4.5 and will be updated in the next release. Legacy drivers remain
available for boards that have not yet migrated.

This is not just a Kconfig change. Out-of-tree boards and overlays can migrate ahead of the
next release by updating both the controller node and its memory-device children:

* Replace :dtcompatible:`st,stm32-xspi` with :dtcompatible:`st,stm32-xspi-controller`,
  :dtcompatible:`st,stm32-ospi` with :dtcompatible:`st,stm32-ospi-controller`, or
  :dtcompatible:`st,stm32-qspi` with :dtcompatible:`st,stm32-qspi-controller`, as appropriate.
  Set ``op-mode`` to ``"MSPI_OP_MODE_CONTROLLER"`` and provide ``clock-frequency`` on the
  controller. Retain the required clocks, pin control, DMA, and port-routing configuration.
* Replace the legacy NOR child compatible with :dtcompatible:`jedec,nor` and any applicable
  vendor-specific compatibles. For devices using :dtcompatible:`st,nor`, also provide the
  required ``st,mem-type`` property.
* Rename ``ospi-max-frequency`` or ``qspi-max-frequency`` to ``mspi-max-frequency``,
  ``spi-bus-width`` to ``mspi-io-mode``, and ``data-rate`` to ``mspi-data-rate`` where applicable.
  The MSPI mode and data-rate properties take string values such as ``"MSPI_IO_MODE_OCTAL"``
  and ``"MSPI_DATA_RATE_DUAL"`` rather than the legacy numeric constants.
* When applicable, configure the memory's command opcodes, command length, dummy cycles, and addressing mode
  for the selected protocol. The legacy ``four-byte-opcodes`` property is not part of the MSPI NOR binding;
  use the new binding's addressing and command configuration instead.
* Preserve flash-region and partition labels referenced by application overlays and chosen
  nodes, and retain the appropriate memory mapping. Update overlays that refer to old node
  names or controller-specific properties.
* For memory-mapped configurations, enable :kconfig:option:`CONFIG_MSPI_MEMMAP` and configure
  the device's ``memmap-config`` property.

The following STM32H573I-DK example shows the XSPI controller and octal DTR NOR changes.
Existing pin control, clocks, DMA, and flash-region/partition definitions are omitted and must
be retained. The command and timing values are specific to this board's flash device.

.. tabs::

   .. group-tab:: Before (CONFIG_FLASH_STM32_XSPI)

      .. code-block:: devicetree

         &xspi1 {
             compatible = "st,stm32-xspi";
             status = "okay";

             ext_flash_ctrl: xspi-flash-controller@0 {
                 compatible = "st,stm32-xspi-nor";
                 reg = <0>;
                 size = <DT_SIZE_M(512)>;
                 ospi-max-frequency = <50000000>;
                 spi-bus-width = <XSPI_OCTO_MODE>;
                 data-rate = <XSPI_DTR_TRANSFER>;
                 four-byte-opcodes;
                 status = "okay";
             };
         };

   .. group-tab:: After (CONFIG_FLASH_MSPI_NOR)

      .. code-block:: devicetree

         &xspi1 {
             compatible = "st,stm32-xspi-controller";
             op-mode = "MSPI_OP_MODE_CONTROLLER";
             clock-frequency = <50000000>;
             st,csbound = <0>;
             status = "okay";

             ext_flash_ctrl: nor-flash-controller@0 {
                 compatible = "st,nor", "mxicy,mx25u", "jedec,nor";
                 reg = <0>;
                 size = <DT_SIZE_M(512)>;
                 mspi-max-frequency = <50000000>;
                 mspi-io-mode = "MSPI_IO_MODE_OCTAL";
                 mspi-data-rate = "MSPI_DATA_RATE_DUAL";
                 mspi-hardware-ce-num = <0>;
                 st,mem-type = "macronix";
                 read-command = <0xee11>;
                 write-command = <0x12ed>;
                 rx-dummy = <20>;
                 command-length = "INSTR_2_BYTE";
                 status = "okay";
             };
         };

See :zephyr_file:`boards/st/stm32h573i_dk/stm32h573i_dk-common.dtsi` for the complete
devicetree, as an example of migrated board.
