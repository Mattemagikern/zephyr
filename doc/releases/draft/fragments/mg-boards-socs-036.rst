::area:: Boards/SoCs
::pr:: 122360

::migration-guide::
The following STM32-based boards have migrated their external serial memory to the MSPI
controller model in this release:

* :zephyr:board:`stm32h573i_dk` and :zephyr:board:`stm32h5f5j_dk` use
  :dtcompatible:`st,stm32-xspi-controller`.
* :zephyr:board:`stm32h735g_disco`, :zephyr:board:`stm32h7b3i_dk`,
  :zephyr:board:`stm32l562e_dk`, and :zephyr:board:`fk7b0m1_vbt6` use
  :dtcompatible:`st,stm32-ospi-controller`.

The migration is ongoing, otherer STM32 boards still use the legacy bindings.
See :ref:`migration_4_5_mspi_stm32` for the migration and deprecation timeline,
and the required configuration changes.
