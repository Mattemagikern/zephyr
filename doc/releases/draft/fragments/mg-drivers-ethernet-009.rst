::area:: Drivers: Ethernet
::pr:: 109671

::migration-guide::
``port_phylink_change`` of the :c:struct:`dsa_api` is now optional.
The DSA driver no longer needs to call :c:func:`net_eth_carrier_on` or
:c:func:`net_eth_carrier_off` on PHY link change, this is now handled by the DSA core.
The ``void *user_data`` argument of ``port_phylink_change`` has been changed to
``const struct device *dev``, so it no longer needs to be cast to obtain the device pointer.
Out-of-tree DSA drivers must update their ``port_phylink_change`` callback to match the new API and
can remove any calls to :c:func:`net_eth_carrier_on` or :c:func:`net_eth_carrier_off` from it.
