::area:: Drivers: Ethernet
::pr:: 87313

::migration-guide::
The Xilinx GEM Ethernet driver (:dtcompatible:`xlnx,gem`) has been switched over to use the
current MDIO and PHY facilities, splitting up the driver's implementation into separate
MDIO and Ethernet MAC drivers. The driver's custom PHY management code has been removed.
The types of Ethernet PHYs supported by the removed custom code, the Marvell Alaska GBit
PHY family and the TI TLK105/DP83822 100 MBit PHYs are both covered by the standard
(:dtcompatible:`ethernet-phy`) driver. The QEMU targets which emulate the Xilinx GEM have
been updated accordingly, as have been the device trees of the Zynq-7000 and ZynqMP /
UltraScale+ SoC families.
