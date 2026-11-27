::area:: Bluetooth Audio
::pr:: 101070

::migration-guide::
CAP

* :c:func:`bt_cap_commander_broadcast_reception_start` now waits for the CAP acceptors to sync to
  the broadcast before completing. This means that if the broadcast source is offline,
  including colocated broadcast sources like the ones created by
  :c:func:`bt_cap_handover_unicast_to_broadcast`, shall be active and have the periodic advertising
  enabled with a configured BASE. For :c:func:`bt_cap_handover_unicast_to_broadcast` the newly
  added :c:member:`bt_cap_handover_cb.unicast_to_broadcast_created` can be used to configure the
  BASE. This also means that any current checks implemented by an application to wait for receive
  state updates indicating successful sync can be removed,
  as :c:func:`bt_cap_commander_broadcast_reception_start` now ensures this when
  :c:member:`bt_cap_commander_cb.broadcast_reception_start` is called. This also applies for
  :c:func:`bt_cap_commander_broadcast_reception_stop` in a similar manner.
* :zephyr:code-sample:`bluetooth_cap_acceptor`,
  :zephyr:code-sample:`bluetooth_cap_handover` and
  :zephyr:code-sample:`bluetooth_cap_initiator` have been moved from
  ``samples/bluetooth/`` to ``samples/bluetooth/audio``.
* :c:func:`bt_cap_initiator_unicast_audio_update` now rejects the API call if it would result in
  no changed states (i.e. the metadata in the parameters are identical to the metadata the server
  have reported to us), and will return ``-EALREADY``.
