::area:: Bluetooth Audio
::pr:: 104219, 104887, 107989, 114835

::migration-guide::
BAP

* :c:member:`bt_bap_stream.codec_cfg` is now ``const``, to better reflect that it is a read-only
  value. Any non-read uses of it will need to be updated with the appropriate operations such as
  :c:func:`bt_bap_stream_config`, :c:func:`bt_bap_stream_reconfig`, :c:func:`bt_bap_stream_enable`
  or :c:func:`bt_bap_stream_metadata`.
* :c:member:`bt_bap_stream.qos` is now ``const``, to better reflect that it is a read-only
  value. Any non-read uses of it will need to be set with the appropriate operations such as
  :c:func:`bt_bap_unicast_group_create`, :c:func:`bt_bap_unicast_group_reconfig`,
  :c:func:`bt_bap_broadcast_source_create` or :c:func:`bt_bap_broadcast_source_reconfig`.
* Almost all API uses of ``struct bt_bap_qos_cfg *`` are now ``const``, which means that once the
  ``qos`` has been stored in a parameter struct like
  :c:struct:`bt_bap_broadcast_source_param` or
  :c:struct:`bt_bap_unicast_group_stream_param`, then the parameter's pointer cannot be used
  to modify the ``qos``, and the actual definition of the struct should be modified instead.
* :c:member:`bt_bap_unicast_group_info.sink_pd` and :c:member:`bt_bap_unicast_group_info.source_pd`
  now reflect the local values defined for the group, and not the values configured for any remote
  ASEs.
* :c:func:`bt_bap_unicast_client_discover` and :c:func:`bt_bap_broadcast_assistant_discover` now
  require that the connection has already gone through the pairing process and meets the security
  requirements of BAP before doing any discovery. In most cases this requires a call to
  :c:func:`bt_conn_set_security` for new devices. Bonded devices that reconnect should not require
  anything.
* Almost all API uses of ``struct bt_audio_codec_cfg *`` are now ``const``, which means that once
  the ``codec_cfg`` has been stored in a parameter struct like
  :c:struct:`bt_bap_stream` or
  :c:struct:`bt_bap_broadcast_source_subgroup_param`, then the parameter's pointer cannot be used
  to modify the ``codec_cfg``, and the actual definition of the struct should be modified instead.
* All BAP roles now require :kconfig:option:`CONFIG_BT_AUDIO_CODEC_CFG_MAX_DATA_SIZE` to be at least
  19 octets, as mandated by the BAP spec.
  If :kconfig:option:`CONFIG_BT_AUDIO_CODEC_CFG_MAX_DATA_SIZE` is set to a lower value when used
  with BAP, then applications need to set it to at least 19 octets. The following Kconfig options
  are affected:

  * :kconfig:option:`CONFIG_BT_ASCS`
  * :kconfig:option:`CONFIG_BT_BAP_UNICAST_SERVER`
  * :kconfig:option:`CONFIG_BT_BAP_UNICAST_CLIENT`
  * :kconfig:option:`CONFIG_BT_BAP_BROADCAST_SOURCE`
  * :kconfig:option:`CONFIG_BT_BAP_BROADCAST_SINK`
  * :kconfig:option:`CONFIG_BT_BAP_SCAN_DELEGATOR`
  * :kconfig:option:`CONFIG_BT_BAP_BROADCAST_ASSISTANT`
* :zephyr:code-sample:`bluetooth_bap_broadcast_assistant`,
  :zephyr:code-sample:`bluetooth_bap_broadcast_sink`,
  :zephyr:code-sample:`bluetooth_bap_broadcast_source`,
  :zephyr:code-sample:`bluetooth_bap_unicast_client` and
  :zephyr:code-sample:`bluetooth_bap_unicast_server` have been moved from
  :zephyr_file:`samples/bluetooth/` to :zephyr_file:`samples/bluetooth/audio`.
* ``bt_bap_stream_ops.configured`` has been renamed to
  :c:member:`bt_bap_stream_ops.codec_configured` and ``bt_bap_stream_ops.qos_set`` has been
  renamed to :c:member:`bt_bap_stream_ops.qos_configured`, to align the names with the ASE states
  defined by the ASCS specification. The callback signatures and the conditions where they are
  called are unchanged, so applications only need to do a search-and-replace from ``configured``
  to ``codec_configured`` and from ``qos_set`` to ``qos_configured`` where the callbacks are
  assigned.

* :c:func:`bt_bap_scan_delegator_mod_src` no longer treats ``metadata_len = 0`` as "keep existing"
  and will now set the metadata length to 0 for the subgroup. To keep existing data, the
  ``metadata_len`` field needs to be set to the existing length and the existing metadata
  shall be copied.
