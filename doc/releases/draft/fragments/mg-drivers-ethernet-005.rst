::area:: Drivers: Ethernet
::pr:: 118881

::migration-guide::
The NuMaker Ethernet driver has been removed together with ``CONFIG_ETH_NUMAKER``. The NuMaker
EMAC is now driven by :kconfig:option:`CONFIG_ETH_NUMAKER_DWC_ETHER_1000`, the generic Synopsys
DesignWare MAC driver, which needs the MDIO controller and the PHY in devicetree. Out-of-tree
boards have to enable the ``mdio`` node with the MDC and MDIO pins in its pinctrl state, add
their PHY to it and point the ``emac`` node at it with ``phy-handle``. The ``phy-addr``
property of :dtcompatible:`nuvoton,numaker-ethernet` has been removed.
