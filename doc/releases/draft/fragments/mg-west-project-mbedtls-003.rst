::area:: West project: mbedtls
::pr:: 108960

::migration-guide::
``CONFIG_PSA_CRYPTO_CLIENT`` has been removed as it was a duplicate of
:kconfig:option:`CONFIG_PSA_CRYPTO`. If you were using it, use
:kconfig:option:`CONFIG_PSA_CRYPTO` instead.
