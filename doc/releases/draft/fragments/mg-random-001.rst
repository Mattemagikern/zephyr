::area:: Random
::pr:: 109914

::migration-guide::
``CONFIG_CTR_DRBG_CSPRNG_GENERATOR`` has been removed.
Use :kconfig:option:`CONFIG_PSA_CSPRNG_GENERATOR` instead.

``CONFIG_CS_CTR_DRBG_PERSONALIZATION`` has been removed. It did not have any effect.
