::area:: MCU Manager
::pr:: 118625

::migration-guide::
The image management client (:kconfig:option:`CONFIG_MCUMGR_GRP_IMG_CLIENT`)
now supports SHA-512 image digests in addition to SHA-256:

* :c:func:`img_mgmt_client_state_write` takes a new ``hash_len`` argument.
  When ``hash`` is not ``NULL``, pass its length in bytes (for example, ``32``
  for SHA-256). Otherwise, pass ``0``.
* :c:struct:`mcumgr_image_data` now stores a variable-length digest: the
  ``hash`` buffer is :c:macro:`IMG_MGMT_CLIENT_HASH_MAX_LEN` (64) bytes, and
  the new ``hash_len`` field holds the actual length. Code that reads ``hash``
  must use ``hash_len`` instead of assuming :c:macro:`IMG_MGMT_DATA_SHA_LEN`.
