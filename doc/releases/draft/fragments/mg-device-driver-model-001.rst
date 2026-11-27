::area:: Device Driver Model
::pr:: 103802

::migration-guide::
The :c:macro:`DEVICE_API` macro is now mandatory for declaring device driver API instances of any
upstream driver class, including in out-of-tree drivers. :c:macro:`DEVICE_API_GET` now asserts
that the API belongs to the requested class, which requires the instance to live in the class's
iterable section. Out-of-tree driver classes that embed an upstream API as their first member
must also declare the relationship with :c:macro:`DEVICE_API_EXTENDS`, so that
:c:macro:`DEVICE_API_GET` for the parent class succeeds on devices implementing the child API.
See :ref:`device_driver_api` for details.
