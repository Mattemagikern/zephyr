::area:: RTIO
::pr:: 114551

::migration-guide::
The cancellation handle changed type from ``struct rtio_sqe *`` to a handle containing
a generation and index into the pool typedefed as :c:type:`rtio_sqe_handle_t`. Unlike a raw
pointer, a handle can be checked (bounds + generation). Cancelling a submission that has already
completed and had its pool slot recycled is now a safe no-op instead of undefined behavior.

:c:func:`rtio_sqe_cancel` now takes the owning RTIO context in addition to the handle, so it can
resolve the handle against the context's SQE pool:

.. code-block:: c

   /* Before */
   int rtio_sqe_cancel(struct rtio_sqe *sqe);

.. code-block:: c

   /* After */
   int rtio_sqe_cancel(struct rtio *r, rtio_sqe_handle_t handle);

The handle out-parameter of :c:func:`rtio_sqe_copy_in_get_handles`, :c:func:`sensor_stream` and
:c:func:`adc_stream` changed from ``struct rtio_sqe **`` to ``rtio_sqe_handle_t *``. Variables that
hold such a handle (including the ``stream_sqe`` field of :c:struct:`sensing_sensor`) must be
retyped to :c:type:`rtio_sqe_handle_t`. Use :c:macro:`RTIO_SQE_HANDLE_INVALID` as the "no
submission" sentinel where code previously compared a handle pointer against ``NULL``.

:c:func:`rtio_sqe_copy_in_get_handles` now writes one handle per copied SQE rather than only
the first, so its out-parameter is an array of ``sqe_count`` handles. Callers that copy in a
single SQE are unaffected; callers that copy in a chain or a transaction and passed the address
of one variable must pass an array that big instead.

:c:func:`rtio_sqe_cancel` marks only the submission its handle names. It never reaches the rest
of a chain or transaction, so cancelling one means cancelling every handle
:c:func:`rtio_sqe_copy_in_get_handles` returned for it. Cancellation stays best effort:
marking a member before it is dispatched does stop it, because the executor checks each
member's status on the way to its iodev, but a member whose predecessor completes before or
while it is being marked may already be running and will then run to completion.

The ``RTIO_SQE_CANCELED`` flag has been deprecated. Cancellation state moved from ``sqe.flags``
into the atomic status word of the owning :c:struct:`rtio_iodev_sqe`, where it is set and
checked atomically. Code that read the flag, typically drivers servicing streaming submissions,
must call :c:func:`rtio_iodev_sqe_is_canceled` instead; reading the flag still compiles (with a
deprecation warning) but no longer reflects cancellation requested at runtime through
:c:func:`rtio_sqe_cancel`. Setting the flag before submit still cancels the submission for as
long as the flag exists, but new code should cancel by handle instead.

Runtime state that the subsystem or a driver mutates after submit moved out of
:c:struct:`rtio_sqe` into the owning :c:struct:`rtio_iodev_sqe`: the mempool buffer binding
(formerly written back to ``sqe->rx.buf`` / ``sqe->rx.buf_len``), the await signal state and
the delay expiry. An iodev must not re-read ``sqe->rx.buf`` to recover a mempool-bound buffer
in a later completion step; calling :c:func:`rtio_sqe_rx_buf` again returns the already bound
buffer. For mempool submissions the field now stays ``NULL`` at runtime, with no compile-time
diagnostic. Caller-provided RX buffers are unaffected.

A Coccinelle semantic patch rewrites the two field moves, ``sqe.await.{ok,callback,userdata}``
into ``rt.await.{ok,callback,userdata}`` and ``sqe.delay.expiry`` into ``rt.delay_expiry``:

.. code-block:: shell

   spatch --sp-file scripts/coccinelle/rtio_sqe_runtime_state.cocci \
          --macro-file scripts/coccinelle/macros.h --dir . --in-place

The mempool buffer has no mechanical rewrite, since replacing a ``sqe.rx`` read with a
:c:func:`rtio_sqe_rx_buf` call needs the buffer length the driver wants and an error path for a
failed allocation. ``MODE=report`` flags those reads instead, in the files that bind a mempool
buffer themselves. Reaching the moved ``await`` and ``delay`` members through a bare
``struct rtio_sqe *`` no longer compiles and needs the wrapper, through
:c:macro:`CONTAINER_OF` or the :c:struct:`rtio_iodev_sqe` the op was handed.

Most applications can migrate automatically with a second Coccinelle semantic patch, which
retypes handle variables, rewrites ``rtio_sqe_cancel(handle)`` into
``rtio_sqe_cancel(ctx, handle)`` (binding ``ctx`` from the producing
``rtio_sqe_copy_in_get_handles`` / ``sensor_stream`` / ``adc_stream`` call), and rewrites reads
of ``RTIO_SQE_CANCELED`` into :c:func:`rtio_iodev_sqe_is_canceled` calls:

.. code-block:: shell

   spatch --sp-file scripts/coccinelle/rtio_sqe_handle.cocci \
          --macro-file scripts/coccinelle/macros.h --dir . --in-place

or, using the wrapper, ``MODE=patch scripts/coccicheck``. Handles stored in a struct field or
cancelled in a different function than they were produced (as in the sensing subsystem), and
handles kept in arrays, fall outside what the patch can bind and must be updated by hand;
``MODE=report`` flags the remaining ``rtio_sqe_cancel()`` call sites and any leftover
``RTIO_SQE_CANCELED`` uses, including writes, which have no mechanical rewrite.
