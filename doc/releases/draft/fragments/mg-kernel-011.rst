::area:: Kernel
::pr:: 108949

::migration-guide::
The type of the expected value parameter for :c:func:`k_futex_wait` is now
``atomic_val_t`` instead of ``int`` to match the type of the value stored
internally by :c:struct:`k_futex`. Callers might have to add an explicit
cast if the compiler does not implicitly cast the value.
