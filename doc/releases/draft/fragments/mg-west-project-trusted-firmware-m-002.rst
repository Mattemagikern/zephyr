::area:: West project: trusted-firmware-m
::pr:: 114762

::migration-guide::
:kconfig:option:`CONFIG_BUILD_WITH_TFM` does not enable :kconfig:option:`CONFIG_MBEDTLS` /
:kconfig:option:`CONFIG_PSA_CRYPTO` anymore. Make sure to enable them explicitly in your build as needed.
