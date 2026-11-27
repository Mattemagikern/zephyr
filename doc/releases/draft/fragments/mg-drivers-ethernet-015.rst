::area:: Drivers: Ethernet
::pr:: 114899

::migration-guide::
:dtcompatible:`infineon,xmc4xxx-ethernet` and :dtcompatible:`wch,ethernet` nodes have been merged
with their parent node. The sibling MDIO nodes are not moved and become children of these
``ethernet`` nodes as a result.

The property ``mdi-port-ctrl`` of ``infineon,xmc4xxx-mdio`` has been moved to the parent node
(:dtcompatible:`infineon,xmc4xxx-ethernet`).

The compatibles ``espressif,esp32-mdio``, ``infineon,xmc4xxx-mdio``, ``nxp,enet-qos-mdio``,
``nxp,s32-gmac-mdio``, ``st,stm32-mdio`` and ``wch,mdio`` have been replaced by
:dtcompatible:`snps,dwmac-mdio`.
