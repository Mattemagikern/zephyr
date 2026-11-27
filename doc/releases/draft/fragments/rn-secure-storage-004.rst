::area:: Secure storage
::pr:: 121707

::release-notes::
Secure Storage:

* ``psa_its_set()`` and ``psa_its_remove()`` now only replace or remove an entry that cannot be
  read back when it is corrupt, and fail with the underlying error otherwise.
* The decrypted data of an entry is now wiped from the subsystem's own buffers before returning.
* The ZMS store now supports 64-bit UIDs through 64-bit ZMS IDs and rejects the UIDs it cannot map
  instead of aliasing them. See the :ref:`migration guide <migration_4.5>` for more information.
