::area:: Build system
::pr:: 116944

::migration-guide::
``west spdx --init`` is deprecated and will be removed in Zephyr 5.0. A build with
:kconfig:option:`CONFIG_BUILD_OUTPUT_META` now asks CMake for the file-based API object model
that ``west spdx`` reads, so generating an SBOM no longer needs the build directory to be
prepared beforehand: build as usual, then run ``west spdx``.
