::area:: Drivers: ESPI
::pr:: 100706

::migration-guide::
ECUSTOM_HOST_SUBS_INTERRUPT_EN has been deprecated in favor of new API that allows fine-grained
enable/disable control of individual eSPI hardware interrupts.
This replaces the current all-or-nothing approach, which is tightly coupled to
CONFIG_ESPI_PERIPHERAL_CUSTOM_OPCODE and single eSPI ACPI HW block instance in all eSPI drivers.
This will be completely removed in the next Zephyr release to give time for transition.
