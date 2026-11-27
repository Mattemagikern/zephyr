::area:: Drivers: Ethernet
::pr:: 114087

::migration-guide::
Ethernet drivers that support LLDP, no longer need to call :c:func:`net_lldp_set_lldpdu` in their
initialization. It is now done by :c:func:`ethernet_init`.
