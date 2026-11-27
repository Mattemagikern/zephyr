::area:: Drivers: ESPI
::pr:: 109519

::migration-guide::
The Microchip XEC eSPI v2 driver (:dtcompatible:`microchip,xec-espi-v2`) has been ported from
MEC172x to also support MEC174x, MEC175x, and MEC165xB. This required several devicetree
changes that affect out-of-tree boards using this binding:

* The ``pcrs`` property has been replaced by ``pcr-scr``, which is now a single integer
  encoded with the ``MCHP_XEC_SCR_ENCODE(reg, bit)`` helper macro instead of a
  ``<reg bit>`` cell pair. Update existing overlays from ``pcrs = <2 19>;`` to
  ``pcr-scr = <MCHP_XEC_SCR_ENCODE(2, 19)>;``.

* On MEC174x/5x/165xB only, ``girqs`` cells are now a single integer per entry produced
  by ``MCHP_XEC_ECIA_GIRQ_ENC(reg, bit)`` rather than a ``<reg bit>`` pair. MEC172x
  continues to use the existing ``MCHP_XEC_ECIA(...)`` form.

* Two new optional properties are available on the eSPI controller node for hosts whose
  address space exceeds 32 bits: ``host-memmap-addr-high`` (host address bits [47:32] for
  memory-mapped logical devices) and ``sram-bar-addr-high`` (host address bits [47:32] for
  both SRAM BARs).

* The ``interrupt-names`` of the eSPI controller and its host-device children have been
  renamed for consistency. Update overlays accordingly:

  * Controller: ``rst`` → ``erst``; ``vwct_0_6`` / ``vwct_7_10`` →
    ``ht_vw_bank0`` / ``ht_vw_bank1``. Two corresponding interrupts
    (``ht_vw_bank0`` / ``ht_vw_bank1``) must be added to the ``interrupts`` array.
  * KBC child: ``kbc_obe`` / ``kbc_ibf`` → ``obe`` / ``ibf``.
  * ACPI EC children: ``acpi_ibf`` / ``acpi_obe`` → ``ibf`` / ``obe``.

* In the MEC5 SoC DTSI (MEC174x/5x/165xB), every host-device child of the eSPI
  controller (mailbox, KBC, ACPI EC, ACPI PM1, port92, glue, EMI, BIOS debug port, etc.)
  now declares the ``ldn`` (logical device number) property required by
  :dtcompatible:`microchip,xec-espi-host-dev`. Out-of-tree boards that override or add
  host-device child nodes for these SoCs must set ``ldn`` on each.
