::area:: Drivers: Wi-Fi
::pr:: 106086

::migration-guide::
In the functions implemented by the :c:struct:`net_wifi_mgmt_offload`, internally
:c:struct:`ethernet_api` and :c:struct:`wifi_mgmt_ops`, a additional argument was added for
a pointer to :c:struct:`net_if`. This api is not directly exposed to the application, so only
out-of-tree drivers need to be updated.
