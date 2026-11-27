::area:: Secure storage
::pr:: 118718

::migration-guide::
The ``psa_its_get*()`` functions can now return ``PSA_ERROR_INVALID_SIGNATURE`` and
``PSA_ERROR_DATA_CORRUPT``, which were previously reported as ``PSA_ERROR_GENERIC_ERROR``.

``psa_its_get()`` called with a ``data_size`` of 0 goes through the usual retrieval path, so
it can now fail, with ``PSA_ERROR_DOES_NOT_EXIST`` for instance, instead of always returning
``PSA_SUCCESS``.
