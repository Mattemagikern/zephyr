::area:: USB
::pr:: 118251

::migration-guide::
The ``get_desc`` callback in :c:struct:`usbd_class_api` now returns ``const void *`` instead of
``void *``, so that a class can keep its array of descriptor pointers in ROM. Out-of-tree
classes must update the return type of their handler.
