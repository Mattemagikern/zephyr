::area:: STM32 Platforms
::pr:: 118318

::migration-guide::
All Ethernet pinctrl nodes for STM32H5 series except :samp:`eth_mdc_{px0}`, :samp:`eth_mdio_{px0}`
and :samp:`eth_pps_out_{px0}` have been renamed to match the Data Sheet names.

The following table indicates the mapping between old and new names and can be used to migrate:

.. list-table::
   :header-rows: 1
   :widths: 30 35 35

   * - Old name
     - New name (``mii`` PHY)
     - New name (``rmii`` PHY)
   * - :samp:`eth_crs_dv_{px0}`
     - *N/A for MII*
     - :samp:`eth_rmii_crs_dv_{px0}`
   * - :samp:`eth_ref_clk_{px0}`
     - *N/A for MII*
     - :samp:`eth_rmii_ref_clk_{px0}`
   * - :samp:`eth_col_{px0}`
     - :samp:`eth_mii_col_{px0}`
     - *N/A for RMII*
   * - :samp:`eth_crs_{px0}`
     - :samp:`eth_mii_crs_{px0}`
     - *N/A for RMII*
   * - :samp:`eth_rx_clk_{px0}`
     - :samp:`eth_mii_rx_clk_{px0}`
     - *N/A for RMII*
   * - :samp:`eth_rx_dv_{px0}`
     - :samp:`eth_mii_rx_dv_{px0}`
     - *N/A for RMII*
   * - :samp:`eth_rx_er_{px0}`
     - :samp:`eth_mii_rx_er_{px0}`
     - *N/A for RMII*
   * - :samp:`eth_rxd0_{px0}`
     - :samp:`eth_mii_rxd0_{px0}`
     - :samp:`eth_rmii_rxd0_{px0}`
   * - :samp:`eth_rxd1_{px0}`
     - :samp:`eth_mii_rxd1_{px0}`
     - :samp:`eth_rmii_rxd1_{px0}`
   * - :samp:`eth_rxd2_{px0}`
     - :samp:`eth_mii_rxd2_{px0}`
     - *N/A for RMII*
   * - :samp:`eth_rxd3_{px0}`
     - :samp:`eth_mii_rxd3_{px0}`
     - *N/A for RMII*
   * - :samp:`eth_tx_clk_{px0}`
     - :samp:`eth_mii_tx_clk_{px0}`
     - *N/A for RMII*
   * - :samp:`eth_tx_en_{px0}`
     - :samp:`eth_mii_tx_en_{px0}`
     - :samp:`eth_rmii_tx_en_{px0}`
   * - :samp:`eth_txd0_{px0}`
     - :samp:`eth_mii_txd0_{px0}`
     - :samp:`eth_rmii_txd0_{px0}`
   * - :samp:`eth_txd1_{px0}`
     - :samp:`eth_mii_txd1_{px0}`
     - :samp:`eth_rmii_txd1_{px0}`
   * - :samp:`eth_txd2_{px0}`
     - :samp:`eth_mii_txd2_{px0}`
     - *N/A for RMII*
   * - :samp:`eth_txd3_{px0}`
     - :samp:`eth_mii_txd3_{px0}`
     - *N/A for RMII*

.. note::
  Pin names now vary depending on whether an MII PHY or an RMII PHY is used; this is indicated
  by property ``phy-connection-type`` (``mii`` or ``rmii``) on the Ethernet node in Devicetree.

  :samp:`{px0}` is a placeholder and should be replaced with actual pin names (e.g., ``pa1``).

  SoCs of the STM32H5Ex/STM32H5Fx line are not affected by this change as they have always used
  the new names since their introduction in Zephyr.
