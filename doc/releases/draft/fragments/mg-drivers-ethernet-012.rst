::area:: Drivers: Ethernet
::pr:: 112924

::migration-guide::
Ethernet and Wi-Fi drivers that use :c:enumerator:`ETHERNET_CONFIG_TYPE_EXTRA_TX_PKT_HEADROOM` to request
extra headroom for transmit packets must now select
:kconfig:option:`CONFIG_NET_L2_ETHERNET_EXTRA_TX_PKT_HEADROOM`.
