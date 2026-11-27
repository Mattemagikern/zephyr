::area:: West project: mbedtls
::pr:: 107052

::migration-guide::
:kconfig:option:`CONFIG_MBEDTLS_SSL_EARLY_DATA` is now an explicit opt-in and is no longer
implicitly enabled by :kconfig:option:`CONFIG_MBEDTLS_SSL_TLS1_3_KEY_EXCHANGE_MODE_PSK_ENABLED`.
Out-of-tree applications or board configurations that rely on TLS 1.3 PSK early data (0-RTT)
must now explicitly enable :kconfig:option:`CONFIG_MBEDTLS_SSL_EARLY_DATA`.
