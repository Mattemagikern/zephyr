::area:: Drivers: DMA
::pr:: 109653

::migration-guide::
:dtcompatible:`silabs,siwx91x-dma` has been renamed :dtcompatible:`silabs,udma`. The Kconfig
options have also been renamed to align with this new name (``DMA_SILABS_SIWX91X`` in
``DMA_SILABS_SIWX91X_UDMA`` and ``DMA_SILABS_SIWX91X_SG_BUFFER_COUNT`` in
``DMA_SILABS_SIWX91X_UDMA_DESCR_COUNT``)

To align with the other drivers, ``GPDMA_SILABS_SIWX91X_DESCRIPTOR_COUNT`` has been renamed in
``DMA_SILABS_SIWX91X_GPDMA_DESCR_COUNT``.
