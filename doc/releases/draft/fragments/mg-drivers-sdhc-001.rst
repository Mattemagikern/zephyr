::area:: Drivers: SDHC
::pr:: 101617

::migration-guide::
Renamed the Kconfig option ``CONFIG_SDHC_STM32_POLLING_SUPPORT`` to
:kconfig:option:`CONFIG_SDHC_STM32_DMA_MODE`. The new symbol enables DMA
(default ``y``); set it to ``n`` to use polling mode.

Renamed the Kconfig option ``CONFIG_SDHC_STM32_SDIO`` to
:kconfig:option:`CONFIG_SDHC_STM32_SDMMC`.

The devicetree compatible ``st,stm32-sdio`` was renamed. Use
:dtcompatible:`st,stm32-sdmmc` instead. With this compatible, the legacy
disk driver and the SDHC driver can target the same node. To migrate to the
SDHC STM32 SDMMC driver, disable the legacy disk driver:

.. code-block:: kconfig

   CONFIG_SDMMC_STM32=n

For :dtcompatible:`st,stm32-sdmmc`, the ``sdhi-on-gpios`` property has been
consolidated into the existing ``pwr-gpios`` property. Replace
``sdhi-on-gpios`` with ``pwr-gpios`` in out-of-tree devicetree nodes.
