::area:: West project: mcuboot
::pr:: 111203

::release-notes::
:kconfig:option:`SB_CONFIG_BOOT_SIGNATURE_KEY_FILE` now accepts a comma-separated list of
key files, embedding the public half of each in the MCUboot bootloader. When more
than one key is given, MCUboot accepts an image signed with any of them -- the
typical use is a development bootloader that boots both development- and
production-signed images, while production bootloaders embed only the production
key. The first entry is the key the application is signed with and the rest are
verification-only public keys. See :ref:`build-signing`.
