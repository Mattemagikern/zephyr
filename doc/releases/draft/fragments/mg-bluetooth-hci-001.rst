::area:: Bluetooth HCI
::pr:: 105834

::migration-guide::
The devicetree compatible ``bflb,bl70x-bt-hci`` has been renamed to
:dtcompatible:`bflb,bt-hci`, now that a single binding covers all Bouffalo Lab
on-chip BLE controllers (BL60x/BL70x/BL70XL). Out-of-tree boards and shields
must update their devicetree nodes accordingly.
