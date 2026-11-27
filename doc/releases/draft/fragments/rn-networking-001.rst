::area:: Networking
::pr:: 116939

::release-notes::
Added tracking of local ports bound through offloaded sockets
(:kconfig:option:`CONFIG_NET_SOCKETS_OFFLOAD_PORT_TRACKING`). Offloaded
sockets bind in the offload engine, outside the ``net_context`` layer, so
:c:func:`net_context_port_in_use` could not see them. The socket layer now
tracks those bindings and provides ``net_socket_port_in_use()`` to check
both native and offloaded ports. mDNS probe port selection and DNS-SD
service checks use this new function.
