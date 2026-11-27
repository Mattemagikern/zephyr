::area:: Drivers: Ethernet
::pr:: 117121

::migration-guide::
Boards with Ethernet interfaces should now enable :kconfig:option:`CONFIG_ETH_DRIVER` by default,
instead of :kconfig:option:`CONFIG_NET_L2_ETHERNET`. The later is now enabled by default when the
former is.
