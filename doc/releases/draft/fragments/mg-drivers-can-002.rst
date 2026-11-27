::area:: Drivers: CAN
::pr:: 110201

::migration-guide::
The Bosch M_CAN driver now solely uses RX FIFO0 for processing received CAN frames, ensuring these
are processed in the order received on the bus. Out-of-tree users may want to update any
``bosch,mram-cfg`` devicetree property overrides to allocate all FIFO elements to RX FIFO0.
