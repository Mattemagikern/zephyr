::area:: West project: trusted-firmware-m
::pr:: 116318

::migration-guide::
:kconfig:option:`CONFIG_TFM_PARTITION_CRYPTO` now depends on
:kconfig:option:`CONFIG_PSA_CRYPTO_PROVIDER_TFM`, meaning you need to enable
:kconfig:option:`CONFIG_PSA_CRYPTO` for the TF-M Crypto partition to get enabled.
