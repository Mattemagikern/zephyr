::area:: Drivers: Ethernet
::pr:: 112788

::migration-guide::
``ETHERNET_PTP`` flag has been removed from :c:enum:`ethernet_hw_caps`.
Use :c:func:`net_eth_get_ptp_clock` to check if the ethernet interface has a PTP clock.
Out-of-tree drivers must remove any references to these flags from their
:c:struct:`ethernet_api` ``get_capabilities`` implementation.
