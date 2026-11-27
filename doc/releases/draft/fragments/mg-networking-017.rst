::area:: Networking
::pr:: 119558

::migration-guide::
The DHCPv4 client now raises ``NET_EVENT_IPV4_DHCP_STOP`` before it gives the
lease up, where it used to raise it last. An application that stops the client
and inspects the interface from that handler now sees the leased address and
the lease's DNS servers still installed; they are taken away after it returns,
the address last. Move such work to the handler for
``NET_EVENT_IPV4_ADDR_DEL``, which is now the final event of a teardown.
