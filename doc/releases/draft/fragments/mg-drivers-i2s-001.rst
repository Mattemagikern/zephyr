::area:: Drivers: I2S
::pr:: 115727

::migration-guide::
:c:func:`i2s_buf_write` now honours the ``timeout`` from the stream configuration when it
allocates the transmit block. It previously waited forever, so the documented ``-EAGAIN``
return was unreachable, as was ``-ENOMEM`` in a multithreaded build. A caller that relied
on the unbounded wait can set ``timeout`` to ``SYS_FOREVER_MS``, but the same field also
bounds the driver's enqueue wait, so no single value reproduces the old combination of an
unbounded allocation and a bounded enqueue.
