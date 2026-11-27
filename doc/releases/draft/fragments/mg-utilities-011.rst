::area:: Utilities
::pr:: 98255

::migration-guide::
The ring buffer "item" API in ``<zephyr/sys/ring_buffer.h>`` has been deprecated in favor of the new
fixed-size queue API in ``<zephyr/sys/ringq.h>``.

Code storing fixed-size items should migrate to :c:struct:`sys_ringq` (see
:ref:`fixed_size_ringq_api`). Code that only used the item API at the byte level should switch to
the byte-mode functions :c:func:`ring_buf_put` / :c:func:`ring_buf_get` calls on the same
:c:struct:`ring_buf`.
