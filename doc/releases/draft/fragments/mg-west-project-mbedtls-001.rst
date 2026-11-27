::area:: West project: mbedtls
::pr:: 116715

::migration-guide::
The following deprecated Kconfig options have been removed:

* ``CONFIG_MBEDTLS_MD`` -> :kconfig:option:`CONFIG_MBEDTLS_MD_C`
* ``CONFIG_MBEDTLS_LMS`` -> :kconfig:option:`CONFIG_MBEDTLS_LMS_C`
* ``CONFIG_MBEDTLS_TLS_VERSION_1_2`` -> :kconfig:option:`CONFIG_MBEDTLS_SSL_PROTO_TLS1_2`
* ``CONFIG_MBEDTLS_DTLS`` -> :kconfig:option:`CONFIG_MBEDTLS_SSL_PROTO_DTLS`
* ``CONFIG_MBEDTLS_TLS_VERSION_1_3`` -> :kconfig:option:`CONFIG_MBEDTLS_SSL_PROTO_TLS1_3`
* ``CONFIG_MBEDTLS_TLS_SESSION_TICKETS`` ->
  :kconfig:option:`CONFIG_MBEDTLS_SSL_SESSION_TICKETS`
* ``CONFIG_MBEDTLS_CTR_DRBG_ENABLED`` -> :kconfig:option:`CONFIG_MBEDTLS_CTR_DRBG_C`
* ``CONFIG_MBEDTLS_HMAC_DRBG_ENABLED`` -> :kconfig:option:`CONFIG_MBEDTLS_HMAC_DRBG_C`

Unlike the removed options, the new ones do not enable their dependencies automatically.
