::area:: Bluetooth Classic
::pr:: 116932

::migration-guide::
The :c:member:`bt_rfcomm_dlc_ops.recv` callback signature has changed from
``void`` to ``int``. Existing implementations must be updated to return ``0``
to preserve previous synchronous behavior, or ``-EINPROGRESS`` to use the new
asynchronous completion path.
