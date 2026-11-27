::area:: MCU Manager
::pr:: 120164

::migration-guide::
:kconfig:option:`CONFIG_MCUMGR_TRANSPORT_UDP_MTU` can no longer exceed
:kconfig:option:`CONFIG_MCUMGR_TRANSPORT_NETBUF_SIZE`, and a larger value now fails at
configuration time. When the buffer is smaller than 1500 bytes, the MTU now defaults to the
buffer size instead of 1500.
