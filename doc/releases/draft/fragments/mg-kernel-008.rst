::area:: Kernel
::pr:: 110059

::migration-guide::
:c:struct:`k_futex` is no longer a kernel object and the corresponding type
:c:enumerator:`K_OBJ_FUTEX` has been removed. Any user-accessible memory can
be used as futex address. The error -EINVAL can no longer happen on futex
operations.
