::area:: Drivers: Sensors
::pr:: 97386

::migration-guide::
:dtcompatible:`st,lps22hh` now ignores the ``odr`` property in favor of the one-shot sampling mode
unless :kconfig:option:`CONFIG_LPS22HH_TRIGGER` is enabled to make use of periodic sampling.
