::area:: Drivers: SDHC
::pr:: 108411

::migration-guide::
:dtcompatible:`litex,mmc` now uses the ``dma-coherent`` devicetree property to indicate that the
controller's DMA accesses are coherent with the CPU.
:kconfig:option:`CONFIG_SDHC_LITEX_LITESDCARD_NO_COHERENT_DMA` is automatically set based on that
property and is no longer user-configurable.
