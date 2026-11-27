::area:: Bluetooth HCI
::pr:: 119710

::migration-guide::
The :ref:`HCI driver API <bt_hci_drivers>` now documents its lifecycle contract.
For its user: :c:func:`bt_hci_open`, :c:func:`bt_hci_close` and :c:func:`bt_hci_send` are
not safe to call concurrently for the same device, :c:func:`bt_hci_send` is only valid on
an open transport, and :c:func:`bt_hci_close` is not called from the receive callback. For
a driver: a failed ``open()`` leaves the transport closed and is not followed by
``close()``, a failed ``close()`` leaves it open, the receive callback is not called any
more once ``close()`` has succeeded, and the driver operations other than ``setup()`` do not
use the Host's HCI command APIs. Out-of-tree HCI drivers, and out-of-tree code that calls
the HCI driver API directly, may have to be changed to follow these rules.
