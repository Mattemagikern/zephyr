::area:: Bluetooth HCI
::pr:: 119615

::migration-guide::
:kconfig:option:`CONFIG_BT_HCI_SET_PUBLIC_ADDR` no longer selects
:kconfig:option:`CONFIG_BT_HCI_SETUP`. Out-of-tree HCI drivers that apply the public
address in their ``setup()`` implementation must now select
:kconfig:option:`CONFIG_BT_HCI_SETUP` themselves; without it the ``setup`` member does
not exist in :c:struct:`bt_hci_driver_api` and the callback is not invoked. The address
is now also available from the time the transport is opened, through
:c:func:`bt_hci_get_public_addr`, allowing drivers to apply it during ``open()`` instead.
