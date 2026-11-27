::area:: Drivers: Ethernet
::pr:: 106086

::migration-guide::
In the functions implemented by the :c:struct:`ethernet_api` a additional argument was added for
a pointer to :c:struct:`net_if`. This api is not directly exposed to the application, so only
out-of-tree drivers need to be updated.
