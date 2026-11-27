::area:: Bluetooth Host
::pr:: 111284

::migration-guide::
:kconfig:option:`CONFIG_BT_OTS_MAX_OBJ_CNT` has been changed from ``hex`` to ``int`` for a
more intuitive type.
Simply modify any hex values like ``0x30`` to their decimal values like ``48``.
