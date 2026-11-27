::area:: Secure storage
::pr:: 118718

::release-notes::
Secure Storage:

* The ``psa_its_get*()`` functions now return ``PSA_ERROR_INVALID_SIGNATURE`` or
  ``PSA_ERROR_DATA_CORRUPT`` for an entry that fails authentication or is malformed,
  instead of ``PSA_ERROR_GENERIC_ERROR``.
* The ITS operations that modify an entry are now serialized, and discarding an entry
  that cannot be read back is logged as a warning.
* ``psa_its_get()`` called with a ``data_size`` of 0 now reports whether the entry exists
  and is valid instead of always returning ``PSA_SUCCESS``.
