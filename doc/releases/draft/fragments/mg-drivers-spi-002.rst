::area:: Drivers: SPI
::pr:: 109653

::migration-guide::
``SPI_SILABS_SIWX91X_GSPI_DMA`` and ``SPI_SILABS_SIWX91X_GSPI_DMA_MAX_BLOCKS`` have been removed.
They are replaced by ``SPI_SILABS_SIWX91X_GSPI_DMA_DESCR_COUNT`` which allow to enable DMA and
configure the descriptor count.
