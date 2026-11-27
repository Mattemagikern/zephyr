::area:: Networking
::pr:: 109798

::migration-guide::
The ``samples/net/wifi/test_certs/rsa2k`` enterprise test certificates have
been removed. TF-PSA-Crypto cannot decrypt their DES-encrypted PKCS#8 private
keys. Use ``samples/net/wifi/test_certs/rsa2k_no_des`` instead, or set
:envvar:`WIFI_TEST_CERTS_DIR` to another AES-encrypted certificate directory.
