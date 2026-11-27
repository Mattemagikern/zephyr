::area:: nRF Platforms
::pr:: 116790

::migration-guide::
The ``owner-id``, ``perm-read``, ``perm-write``, ``perm-execute``, ``perm-secure`` and
``non-secure-callable`` properties of :dtcompatible:`nordic,owned-memory` and
:dtcompatible:`nordic,owned-partitions` have been removed. Use ``nordic,access`` instead, e.g.
``<NRF_OWNER_ID_APPLICATION NRF_PERM_RW>``. The owner is no longer implicit: an omitted
``owner-id`` used to mean the domain being compiled, so it must now be named explicitly.
