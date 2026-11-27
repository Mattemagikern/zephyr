::area:: Bluetooth HCI
::pr:: 119616

::migration-guide::
The HCI driver ``setup()`` op, :c:func:`bt_hci_setup`, :c:struct:`bt_hci_setup_params`,
:kconfig:option:`CONFIG_BT_HCI_SETUP` and the ``bt_h4_vnd_setup()`` hook of the H:4 driver are
deprecated and will be removed two releases from now. A driver that implements ``setup()``
moves that work into :c:member:`bt_hci_driver_api.open`:

* Send the vendor-specific commands with :c:func:`bt_hci_lockstep_cmd_send_sync`, over the
  driver's own transport, instead of with the Host's command APIs, and feed every received
  packet to :c:func:`bt_hci_lockstep_feed`. The helpers of
  :file:`include/zephyr/bluetooth/hci_pkt.h` frame the commands. After resetting the
  controller by other means than an HCI command, call :c:func:`bt_hci_lockstep_reset`.
* Read the public address with :c:func:`bt_hci_get_public_addr` instead of taking it from
  :c:member:`bt_hci_setup_params.public_addr`. As there, it is ``BT_ADDR_ANY``, and not
  ``BT_ADDR_NONE``, when no public address was set.
* Remove ``select BT_HCI_SETUP`` from the driver's Kconfig.

The initialization then also runs in a build without a Host, where ``setup()`` was never
called. With a Host it runs in the thread that calls :c:func:`bt_enable`, before that function
returns, also when a ready callback is passed; ``setup()`` ran from the system workqueue in
that case. This holds for every in-tree driver that implemented ``setup()``, as they have all
been converted.
