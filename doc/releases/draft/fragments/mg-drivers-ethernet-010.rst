::area:: Drivers: Ethernet
::pr:: 110435

::migration-guide::
The MAC address of ethernet interfaces is now checked for validity, when bringing the interface up
with :c:func:`net_if_up`. If the MAC address is invalid, the interface will fail to come up and an
error will be logged. This check is done before the ``start`` function of the
:c:struct:`ethernet_api` is called. This also applies to native wifi drivers.
