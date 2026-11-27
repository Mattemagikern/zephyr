::area:: Drivers: Ethernet
::pr:: 109265

::migration-guide::
The Kconfig option ``CONFIG_ETH_NATIVE_TAP_PTP_CLOCK`` has been replaced by
:kconfig:option:`CONFIG_PTP_CLOCK_NATIVE`. A new compatible
:dtcompatible:`zephyr,native-ptp-clock` has been added for the native_sim PTP clock driver.
:kconfig:option:`CONFIG_PTP_CLOCK_NATIVE` is enabled by default when the
:dtcompatible:`zephyr,native-ptp-clock` compatible is present.
