::area:: Drivers: CAN
::pr:: 120266

::migration-guide::
The CAN bus network driver (:kconfig:option:`CONFIG_NET_CANBUS`) now defines a network interface
for each CAN controller device defined with :c:macro:`CAN_DEVICE_DT_DEFINE` or
:c:macro:`CAN_DEVICE_DT_INST_DEFINE`, instead of one for the ``zephyr,canbus`` chosen node.
