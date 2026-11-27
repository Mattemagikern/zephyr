::area:: Networking: Ethernet
::pr:: 110582

::migration-guide::
Ethernet drivers providing RX timestamps must now call
:c:func:`net_pkt_set_rx_timestamping` after storing a valid timestamp in the received packet.
AF_PACKET sockets use :c:func:`net_pkt_is_rx_timestamping` as the sole indication that
``SO_TIMESTAMPING`` control data is valid. Out-of-tree drivers that only populate
``pkt->timestamp`` must be updated or their RX timestamps will not be passed to
socket applications.
