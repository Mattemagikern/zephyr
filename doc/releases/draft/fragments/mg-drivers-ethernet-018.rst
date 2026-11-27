::area:: Drivers: Ethernet
::pr:: 115952

::migration-guide::
The NXP ENET-QOS Ethernet controller (:dtcompatible:`nxp,enet-qos`) devicetree structure has been
flattened to match other similar controllers. The clocks, interrupts, ``pinctrl-0``,
``pinctrl-names``, ``phy-handle`` and MAC address properties now live directly on the parent
``nxp,enet-qos`` node instead of a separate child MAC node. The ``nxp,enet-qos-mac`` compatible
and its ``enet_mac`` node have been removed.
Out-of-tree boards using this controller must move the properties from the
old ``enet_mac`` node up to the ``enet`` node.

The Kconfig option ``CONFIG_ETH_NXP_ENET_QOS_MAC_UNIQUE_MAC_ADDRESS`` has been renamed to
:kconfig:option:`CONFIG_ETH_NXP_ENET_QOS_UNIQUE_MAC_ADDRESS`. Configurations setting the old
name must be updated to use the new one.
