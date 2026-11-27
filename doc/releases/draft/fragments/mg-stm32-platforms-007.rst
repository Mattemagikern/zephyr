::area:: STM32 Platforms
::pr:: 110546

::migration-guide::
:dtcompatible:`st,hci-stm32wba` and :dtcompatible:`st,stm32wba-ieee802154` nodes
(with nodelabels ``bt_hci_wba`` and ``ieee802154`` respectively) are now
children of a top-level :dtcompatible:`st,stm32wba-radio` node with nodelabel
``radio``. The ``interrupts`` property is now set on the ``&radio`` node instead
of being duplicated on both ``&bt_hci_wba`` and ``&ieee802154`` nodes. Out-of-tree
boards which modified the ``interrupts`` property on either node must be updated
to set the property on the top-level ``&radio`` node instead.
