::area:: Boards/SoCs
::pr:: 118942

::migration-guide::
On NXP S32K148, the ENET nodes ``enet`` (:dtcompatible:`nxp,enet`), ``enet_mac``
(:dtcompatible:`nxp,enet-mac`), ``enet_mdio`` (:dtcompatible:`nxp,enet-mdio`) and
``enet_ptp_clock`` (:dtcompatible:`nxp,enet-ptp-clock`) are now ``disabled`` by default instead
of ``okay``. Out-of-tree boards that use Ethernet must set ``status = "okay"`` on these nodes.
