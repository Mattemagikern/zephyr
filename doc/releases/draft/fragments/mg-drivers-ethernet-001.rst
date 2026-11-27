::area:: Drivers: Ethernet
::pr:: 113315

::migration-guide::
The WIZnet Ethernet drivers now share one set of Kconfig options. Replace
``CONFIG_ETH_W5500_*``, ``CONFIG_ETH_W6100_*`` and ``CONFIG_ETH_W6300_*`` with the matching
``CONFIG_ETH_WIZNET_*`` option.
