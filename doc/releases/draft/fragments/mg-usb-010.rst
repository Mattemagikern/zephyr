::area:: USB
::pr:: 117615

::migration-guide::
``CONFIG_USBD_CDC_ACM_BUF_POOL`` has been removed. The CDC ACM implementation
now allocates bulk IN and OUT transfer buffers from per-instance pools. The
device tree properties ``tx-fifo-size`` or ``rx-fifo-size`` determine the
pool sizes.
