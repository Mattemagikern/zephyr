::area:: Networking
::pr:: 111361

::migration-guide::
:kconfig:option:`CONFIG_NET_L2_PTP` has been deprecated and replaced by
:kconfig:option:`CONFIG_NET_L2_PTP_TIMESTAMPING`. The new option more accurately describes the
feature it enables. Applications or board configurations that explicitly enable
:kconfig:option:`CONFIG_NET_L2_PTP` should be updated to use
:kconfig:option:`CONFIG_NET_L2_PTP_TIMESTAMPING` instead.
