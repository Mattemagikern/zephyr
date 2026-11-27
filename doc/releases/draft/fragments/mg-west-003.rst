::area:: West
::pr:: 111906

::migration-guide::
The ``openocd`` runner now selects a debug adapter by serial number through the
canonical ``-i``/``--dev-id`` option, like the other runners. The previous
``--serial`` option is deprecated and kept as an alias; it maps onto the same
mechanism (the value is still passed to the OpenOCD config as
``_ZEPHYR_BOARD_SERIAL``). Update any scripts to use ``west flash -i <serial>``.
