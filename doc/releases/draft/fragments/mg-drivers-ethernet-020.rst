::area:: Drivers: Ethernet
::pr:: 113235

::migration-guide::
The Synopsys DesignWare MAC driver now filters multicast by default
(:kconfig:option:`CONFIG_ETH_DWC_ETHER_MULTICAST_FILTER`), so only multicast for the addresses
the network stack has joined is received. Disable this option to receive all multicast, as
before.
