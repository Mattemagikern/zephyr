::area:: USB
::pr:: 102491

::migration-guide::
USB control transfer callbacks ``control_to_host`` in :c:struct:`usbd_class_api` and
``to_host`` in :c:struct:`usbd_vreq_node` are now expected to allocate the data stage buffer
themselves. This allows allocating only as much memory as is actually needed which makes
the worst case memory usage dependent on the handlers implementation and not on tainted wLength
value coming from host. Out-of-tree class and vendor handlers need to be updated.
