::area:: Drivers: Ethernet
::pr:: 117068

::migration-guide::
The native_sim TAP ethernet driver is now instantiated from devicetree using the
:dtcompatible:`zephyr,native-tap` compatible. Each interface is defined by a devicetree
node instead of the ``CONFIG_ETH_NATIVE_TAP_INTERFACE_COUNT`` Kconfig option, which has been
removed. Multiple interfaces are created by adding multiple nodes. The following Kconfig
options have been removed and replaced by devicetree properties:

* ``CONFIG_ETH_NATIVE_TAP_DRV_NAME`` -> the ``host-interface`` property.
* ``CONFIG_ETH_NATIVE_TAP_MAC_ADDR`` -> the ``local-mac-address`` property.
* ``CONFIG_ETH_NATIVE_TAP_RANDOM_MAC`` -> the ``zephyr,random-mac-address`` property.

The ``--eth-if``, ``--mac-addr``, ``--ipv4-addr``, ``--ipv4-gw`` and ``--ipv4-nm`` command
line options are still supported and apply to the first interface. Per-interface variants
named ``<node>_eth-if``, ``<node>_mac-addr``, etc. have been added for the remaining
interfaces.
