::area:: Networking
::pr:: 115307

::migration-guide::
On :c:func:`net_if_down`, the multicast addresses are no longer cleared from the interface.
A leave message is still sent, but the addresses are retained in the interface's multicast list
and will be rejoined when the interface is brought back up.
This allows applications to bring the interface down and up without losing the multicast
addresses.
