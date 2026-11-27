::area:: Bluetooth Audio
::pr:: 110607

::migration-guide::
VOCS

* The VOCS client now requires automatic CCC (Client Characteristic Configuration) discovery.
  :kconfig:option:`CONFIG_BT_VOCS_CLIENT` now depends on :kconfig:option:`CONFIG_BT_GATT_AUTO_DISCOVER_CCC`.
  Applications using VOCS client must ensure that CCC auto-discovery support is enabled.
