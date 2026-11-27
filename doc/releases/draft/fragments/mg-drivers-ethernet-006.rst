::area:: Drivers: Ethernet
::pr:: 108952

::migration-guide::
``port_generate_random_mac`` of the :c:struct:`dsa_api` got removed. Also
:c:struct:`dsa_port_config` now uses :c:struct:`net_eth_mac_config` to set the MAC address.
``mac_addr`` and ``use_random_mac_addr`` members of :c:struct:`dsa_port_config` were removed.
Out-of-tree DSA drivers must update their port configuration code to use the new API and
structures.
