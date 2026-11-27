::area:: Build system
::pr:: 116714

::migration-guide::
The CMake ``flash``, ``debug``, ``debugserver``, ``attach`` and ``rtt`` targets have been
removed. Use ``west flash``, ``west debug``, ``west debugserver``, ``west attach`` and
``west rtt`` instead. The emulation ``run`` and ``debugserver`` targets are unaffected.

The ``WEST_DIR`` build system variable is no longer used.
