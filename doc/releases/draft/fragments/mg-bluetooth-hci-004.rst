::area:: Bluetooth HCI
::pr:: 107850

::migration-guide::
The HCI driver :c:member:`bt_hci_driver_api.open` callback no longer has a ``recv`` parameter;
rather the common HCI driver layer code takes care of managing this as part of the common
data struct. There is a new :c:func:`bt_hci_recv` API for drivers to pass data the higher
layer (e.g. the Bluetooth Host stack). For drivers that need access to any error from recv()
(most don't) there's also a new :c:func:`bt_hci_recv_err` API that leaves the responsibility
of unrefing the buffer to the caller in case of error situations.
