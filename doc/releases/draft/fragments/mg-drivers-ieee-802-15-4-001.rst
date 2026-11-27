::area:: Drivers: IEEE 802.15.4
::pr:: 117327

::migration-guide::
The ``IEEE802154_HW_SLEEP_TO_TX`` radio capability, deprecated since Zephyr 3.6, has been
removed and the capability bits above it renumbered. Every in-tree driver supports
transmitting directly from a low-power state, so the capability conveyed no information;
the OpenThread platform now always advertises ``OT_RADIO_CAPS_SLEEP_TO_TX`` and allows
transmission from the sleep state. Out-of-tree drivers advertising the capability simply
drop it.
