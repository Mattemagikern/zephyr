::area:: Common Architecture Interface
::pr:: 115581

::migration-guide::
Both :c:func:`arch_mem_map` and :c:func:`arch_mem_unmap` have changed from
returning ``void`` to ``int`` so that the caller can react to error code when
assertion is disabled. If assertion is enabled, it currently retains mostly
the previous behavior of halting the system.
