::area:: Build system
::pr:: 119947

::migration-guide::
Support for C standard versions older than C17 has been removed after having
been deprecated. The Kconfig options ``CONFIG_STD_C11``, ``CONFIG_STD_C99`` and ``CONFIG_STD_C90``
have been removed. Use C17 or higher when compiling Zephyr.
