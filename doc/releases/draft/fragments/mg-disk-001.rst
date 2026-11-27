::area:: Disk
::pr:: 117809

::migration-guide::
:kconfig:option:`CONFIG_NVME_REQUEST_TIMEOUT` is documented and ranged in
seconds. The NVMe request timeout path previously compared that value against
:c:func:`k_uptime_get_32` milliseconds without converting, so the default of
``5`` expired after about 5 ms instead of 5 seconds. The driver now converts
with ``MSEC_PER_SEC`` before scheduling and expiry checks. Review any
non-default setting if the application depended on the former short timeout
behavior.
