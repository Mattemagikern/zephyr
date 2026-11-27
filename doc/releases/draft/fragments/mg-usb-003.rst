::area:: USB
::pr:: 108840

::migration-guide::
When host issues control transfer with data stage from host to device, the USB control transfer
callbacks ``control_to_dev`` in :c:struct:`usbd_class_api` and ``to_dev`` in
:c:struct:`usbd_vreq_node` are now called with NULL ``buf`` before data stage is received.
This allows the stack to return STALL during data stage. Out-of-tree class and vendor handlers
need to be updated.
