::area:: Secure storage
::pr:: 121707

::migration-guide::
When :kconfig:option:`CONFIG_SECURE_STORAGE_64_BIT_UID` is enabled, the ZMS store
(:kconfig:option:`CONFIG_SECURE_STORAGE_ITS_STORE_IMPLEMENTATION_ZMS`) now enables
:kconfig:option:`CONFIG_ZMS_ID_64BIT` by default, which changes the ZMS storage format.
Set it back to ``n`` to keep compatibility with entries written by earlier versions.
The store now also rejects UIDs with bit 30 or 31 set (which are reserved for the caller ID)
with ``PSA_ERROR_INVALID_ARGUMENT`` instead of silently aliasing them.

``psa_its_set()`` and ``psa_its_remove()`` no longer replace or remove an entry that fails to
be read back for a reason other than failed authentication or corruption, such as a key
provider or storage error. They return that error instead.
