::area:: Drivers: Counter
::pr:: 120632

::migration-guide::
The NXP RTC counter drivers (:dtcompatible:`nxp,rtc`, :dtcompatible:`nxp,lpc-rtc` and
:dtcompatible:`nxp,imx-snvs-rtc`) now return ``-ETIME`` instead of ``-EINVAL`` from
:c:func:`counter_set_channel_alarm` when an absolute alarm is set too late, matching the counter
API contract. Applications that checked for ``-EINVAL`` on the late-absolute-alarm path must check
for ``-ETIME`` instead. An absolute target equal to the current counter value is now also reported
as late (``-ETIME``), and the late check no longer applies to relative alarms.
