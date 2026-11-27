::area:: Storage
::pr:: 119069

::migration-guide::
The ``fs_off`` element of :c:struct:`flash_sector` has been changed from type ``off_t`` to
``ptrdiff_t``. This should make all platforms and toolchains use the native machine register size
and not vary based on the POSIX ``off_t`` type inherited from the C library. Picolibc 1.8.12
always defines ``off_t`` as a 64-bit integer, even on 32-bit platforms; this change effectively
returns the struct to the previous layout when using this C library. For older Picolibc versions
and all other supported C libraries, ``ptrdiff_t`` uses the same underlying C type as ``off_t``;
this change is intended to preserve the undering C type used for ``fs_off`` across the Picolibc
update.
