::area:: Networking: gPTP
::pr:: 112083

::migration-guide::
Removed ``CONFIG_NET_GPTP_CLOCK_ACCURACY_*``, users need to make sure right gPTP clock
accuracy value configured in :kconfig:option:`CONFIG_NET_GPTP_CLOCK_ACCURACY`.
