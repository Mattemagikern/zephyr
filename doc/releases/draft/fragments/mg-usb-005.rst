::area:: USB
::pr:: 111508

::migration-guide::
The Espressif USB-OTG full-speed controller compatible ``espressif,esp32-usb-otg`` has been
renamed to :dtcompatible:`espressif,esp32-usb-otg-fs`. The internal PHY D+/D- pad numbers are
now provided through the ``phy-dp-pin`` and ``phy-dm-pin`` properties. Out-of-tree devicetrees
using the old compatible must update the node compatible and add the two pin properties.
