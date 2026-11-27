::area:: Bluetooth Host
::pr:: 116659

::migration-guide::
When :kconfig:option:`CONFIG_BT_GATT_AUTO_READ_CENTRAL_ADDR_RES` is enabled (the
default when possible), the host reads the Central Address Resolution characteristic
of a bonded peer once when the bond is created, and :c:func:`bt_le_adv_start`,
:c:func:`bt_le_ext_adv_create` and :c:func:`bt_le_ext_adv_update_param` now fail
with ``-ENOTSUP`` when :c:enumerator:`BT_LE_ADV_OPT_DIR_ADDR_RPA` is used towards a
peer known not to support address resolution. Such a peer cannot resolve the target
address, so it would never respond to the advertising. Applications that need to know
in advance can read the same answer with :c:func:`bt_le_bond_addr_res_support`, and
reach those peers with directed advertising towards their identity address instead.
Disabling the option restores the previous behavior.
