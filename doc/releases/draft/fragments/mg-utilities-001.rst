::area:: Utilities
::pr:: 116047

::migration-guide::
The ring buffer API has been reworked to reduce the :c:struct:`ring_buf` size and to make the
bookkeeping path more efficient. To accommodate these changes, the zero-copy claim/finish API
(``ring_buf_put_claim()`` / ``ring_buf_put_finish()`` and their ``get`` counterparts) has been
replaced by the non-stacking :c:func:`ring_buf_put_ptr` and :c:func:`ring_buf_get_ptr`.

The legacy claim/finish API is still available, but only when
:kconfig:option:`CONFIG_RING_BUFFER` is enabled. New code should use the ``_ptr`` API
directly.

Enabling :kconfig:option:`CONFIG_RING_BUFFER` selects the legacy ring buffer header, which
also brings back the other deprecated symbols that are absent from the default header: the entire
item API (:c:func:`ring_buf_item_init`, :c:func:`ring_buf_item_put`, :c:func:`ring_buf_item_get`,
:c:func:`ring_buf_item_space_get`, ``RING_BUF_ITEM_DECLARE*`` and ``RING_BUF_ITEM_SIZEOF``) and
``ring_buf_internal_reset()``. Out-of-tree code that still uses any of these fails to compile with
no other hint; enabling this option is the switch that restores them while the code is migrated to
:c:struct:`sys_ringq` and the ``_ptr`` API.

:c:func:`ring_buf_get` no longer accepts a ``NULL`` destination to discard data in the default
(slim) build; passing ``NULL`` is only tolerated when :kconfig:option:`CONFIG_RING_BUFFER` is
enabled. To drop data without a destination buffer, advance the read index directly with
:c:func:`ring_buf_consume`, for example
``ring_buf_consume(rb, MIN(count, ring_buf_size_get(rb)))``.

Advanced use cases such as **speculative-write-then-cancel** and **backfilling** (modifying a
previously written header before committing) now rely on the trailing ``offset`` parameter of
:c:func:`ring_buf_put_ptr` and :c:func:`ring_buf_get_ptr`. The offset is the number of bytes past
the current write (or read) index that the caller has already tentatively reserved, wrapping
handled internally. You lay out successive regions by passing an increasing offset, leaving the
real ring buffer unmodified, and only advance it with :c:func:`ring_buf_commit` (or
:c:func:`ring_buf_consume`).
If any step fails you simply return without committing, which is the equivalent of the old
``ring_buf_put_finish(rb, 0)`` cancellation.

For example, the following claim/finish code:
