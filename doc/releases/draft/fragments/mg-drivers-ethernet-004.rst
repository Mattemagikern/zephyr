::area:: Drivers: Ethernet
::pr:: 107352

::migration-guide::
The ``pinctrl-0`` and ``pinctrl-names`` devicetree properties for the
:dtcompatible:`nxp,enet-mac` need to be moved from the MAC node to the parent Ethernet controller
node.
