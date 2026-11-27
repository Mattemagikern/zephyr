::area:: Bluetooth Host
::pr:: 116711

::migration-guide::
``CONFIG_BT_AUTO_PHY_UPDATE`` has been removed. Use the per-role ``BT_AUTO_PHY_CENTRAL``
and ``BT_AUTO_PHY_PERIPHERAL`` choices instead. ``=n`` does not translate to dropping the
option: the central choice defaults to :kconfig:option:`CONFIG_BT_AUTO_PHY_CENTRAL_2M`, so
both roles must be set to ``_NONE`` explicitly.
