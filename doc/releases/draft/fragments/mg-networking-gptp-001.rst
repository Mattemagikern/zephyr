::area:: Networking: gPTP
::pr:: 108648

::migration-guide::
Converted ``int port`` to ``uint16_t gptp_port`` in
:c:struct:`ethernet_context` to make it clear that the field used only
by the gPTP stack to store the gPTP port number.

Used ``uint16_t`` for ``nb_ports`` in :c:struct:`gptp_default_ds` per
IEEE 1588 standard.

Removed ``net_eth_get_ptp_port`` and ``net_eth_set_ptp_port``.
New :c:func:`gptp_get_port_number` and :c:func:`gptp_set_port_number`
can be used instead.
