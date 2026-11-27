::area:: Drivers: Serial/UART
::pr:: 115112

::migration-guide::
The :dtcompatible:`brcm,bcm2711-aux-uart` devicetree binding has been removed in favour of
:dtcompatible:`brcm,bcm283x-aux-uart`. Nodes must add :dtcompatible:`ns16550` as a compatible,
replace the ``clocks`` property with ``clock-frequency``, and specify ``reg-shift = <2>``.
The dedicated BCM2711 auxiliary UART driver has been removed in favour of the generic NS16550
driver, which now provides support for the Broadcom BCM283x auxiliary UART through vendor-specific
extensions.
