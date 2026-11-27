::area:: MCU Manager
::pr:: 120352

::migration-guide::
:kconfig:option:`CONFIG_MCUMGR_TRANSPORT_UART_MTU` and
:kconfig:option:`CONFIG_MCUMGR_TRANSPORT_SHELL_MTU` have been deprecated. They only set the value
returned by the deprecated ``get_mtu`` callback, which the SMP layer does not use: serial frames
are limited to 127 bytes and the maximum SMP packet size is set by
:kconfig:option:`CONFIG_MCUMGR_TRANSPORT_NETBUF_SIZE`. Remove them from configuration files; a
deprecation warning is emitted when they are changed from their default.

The ``get_mtu`` member of :c:struct:`smp_transport_api_t` and the
:c:type:`smp_transport_get_mtu_fn` type have been deprecated, as the SMP layer never calls
them. Out-of-tree code should stop setting or calling ``get_mtu``. Transports remain
responsible for fragmenting outgoing packets in their ``output`` function (see
:c:type:`smp_transport_out_fn`).
