::area:: West project: mbedtls
::pr:: 110328

::release-notes::
Added :kconfig:option:`CONFIG_TF_PSA_CRYPTO_DISPATCH_DIR`, which enables TF-PSA-Crypto to use
custom implementations of crypto operation dispatch. This makes hardware acceleration of
cryptographic operations possible by using an accelerator-aware dispatch implementation.
