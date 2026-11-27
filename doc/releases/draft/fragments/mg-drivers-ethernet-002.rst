::area:: Drivers: Ethernet
::pr:: 108136

::migration-guide::
``ETHERNET_CONFIG_TYPE_T1S_PARAM`` and the related ``NET_REQUEST_ETHERNET_SET_T1S_PARAM`` has
been removed. :c:func:`phy_set_plca_cfg` together with :c:func:`net_eth_get_phy` should be
used instead to set these parameters.
