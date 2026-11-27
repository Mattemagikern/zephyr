::area:: Bluetooth Host
::pr:: 116743

::migration-guide::
The deprecated ``CONFIG_BT_CONN_TX_MAX`` Kconfig option has been removed. It has been
deprecated since Zephyr 4.2, and the number of pending TX buffers with a callback always
follows :kconfig:option:`CONFIG_BT_BUF_ACL_TX_COUNT`.
