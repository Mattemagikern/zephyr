::area:: Networking
::pr:: 119558

::release-notes::
The DHCPv4 client now takes the leased address, the lease's DNS servers and
the gateway it installed off the interface on every path that gives a lease
up, and waits about ten seconds before restarting after a refused request
or a declined address.
