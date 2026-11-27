::area:: Bluetooth HCI
::pr:: 120078

::migration-guide::
An H:4 vendor extension configures its controller from :c:func:`bt_h4_vnd_open`, declared in
:zephyr_file:`include/zephyr/drivers/bluetooth/h4.h`, which the H:4 driver calls at the end of
its ``open()`` with a lockstep helper for the extension's commands, instead of from
``bt_h4_vnd_setup()`` and the ``setup()`` op. The extension selects
:kconfig:option:`CONFIG_BT_H4_VND_OPEN` in place of :kconfig:option:`CONFIG_BT_HCI_SETUP` and
reads the public address with :c:func:`bt_hci_get_public_addr` instead of taking it from the
setup parameters. ``bt_h4_vnd_setup()`` is still called through the ``setup()`` op by an
extension that keeps selecting :kconfig:option:`CONFIG_BT_HCI_SETUP`.
