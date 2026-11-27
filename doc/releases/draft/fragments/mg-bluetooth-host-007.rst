::area:: Bluetooth Host
::pr:: 117622

::migration-guide::
:c:member:`bt_le_ext_adv_info.sid` is now being set to ``BT_GAP_SID_INVALID`` for legacy
advertising sets, as SIDs are only valid for extended advertising sets. Applications should not
expect the :c:member:`bt_le_adv_param.sid` to be applied for legacy advertising sets.

:c:member:`bt_le_ext_adv_info.sid` now reflects the SID given to
:c:func:`bt_le_ext_adv_update_param`. Previously it kept the value from
:c:func:`bt_le_ext_adv_create` even though the controller applied the new one.
