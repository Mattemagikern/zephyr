::area:: Build system
::pr:: 116748

::migration-guide::
``CONFIG_BUILD_NO_GAP_FILL`` has been removed. Gap filling is opt-in through
:kconfig:option:`CONFIG_BUILD_OUTPUT_HEX_GAP_FILL` and
:kconfig:option:`CONFIG_BUILD_OUTPUT_S19_GAP_FILL`, so simply drop the option.

:file:`cmake/app/boilerplate.cmake` has been removed. Applications still
including it directly must start their :file:`CMakeLists.txt` with
``find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})`` instead.

Board revision Kconfig fragments named :file:`<board>_<revision>.conf` are no
longer read. Rename them to :file:`<board>_<revision>_defconfig`.

``zephyr_code_relocate(FILES ...)`` no longer expands wildcard patterns, and
now fails on one. Expand the pattern with ``file(GLOB ...)`` and pass the
resulting file names instead.
