::area:: Kernel
::pr:: 106885

::migration-guide::
``_k_neg_eagain`` has been renamed to ``_errno_neg_egain`` as ``errno`` has been migrated out of
kernel into ``lib/libc/common``.
