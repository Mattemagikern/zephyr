::area:: Bluetooth Audio
::pr:: 102430, 103350

::migration-guide::
CCP

* :c:member:`bt_tbs_client_cb.technology` has changed the ``value`` parameter from ``uint32_t``
  to ``enum bt_bearer_tech``. Applications using this application should switch the type.
* All ``BT_TBS_TECHNOLOGY_*`` values like ``BT_TBS_TECHNOLOGY_3G`` are renamed to
  ``BT_BEARER_TECH_*`` like ``BT_BEARER_TECH_3G``. Applications can do search-and-replace from
  ``BT_TBS_TECHNOLOGY`` to ``BT_BEARER_TECH``. Additionally the values are now defined in
  :zephyr_file:`include/zephyr/bluetooth/assigned_numbers.h` instead of
  :zephyr_file:`include/zephyr/bluetooth/audio/tbs.h`.
* ``bt_tbs_register_param.supported_features`` has been renamed to
  :c:member:`bt_tbs_register_param.optional_opcodes`. Applications can do a simple
  search-and-replace for ``supported_features`` to ``optional_opcodes``.
  Additionally the ``BT_TBS_FEATURE_*`` macros have been changed to ``BT_TBS_OPTIONAL_OPCODE_*``.
  Applications can do a simple search-and-replace for ``BT_TBS_FEATURE_`` to
  ``BT_TBS_OPTIONAL_OPCODE_``.
* :zephyr:code-sample:`bluetooth_ccp_call_control_client` and
  :zephyr:code-sample:`bluetooth_ccp_call_control_server` have been moved from
  ``samples/bluetooth/`` to ``samples/bluetooth/audio``.
