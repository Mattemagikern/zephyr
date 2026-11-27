::area:: Bluetooth HCI
::pr:: 107850

::migration-guide::
Bluetooth HCI drivers now have to provide a mandatory common struct as the first field of
their data (:c:struct:`bt_hci_driver_data`) and config (:c:struct:`bt_hci_driver_config`)
structs.
