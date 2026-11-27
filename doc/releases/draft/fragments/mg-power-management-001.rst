::area:: Power management
::pr:: 118977, 120960

::migration-guide::
The new, default-off experimental option
:kconfig:option:`CONFIG_PM_EARLY_SYSTEM_TIMER_RESUME` selects the sequence introduced by
:github:`118977`: SoC post-ops, system timer idle-exit, device resume, then PM exit notifications.
Enable it only after validating platform dependencies and timeout callbacks. SoC post-ops
run before device resume, and timer idle-exit may invoke :c:struct:`k_timer` expiry functions
while devices are still suspended. Without this option, timer-based :c:func:`k_busy_wait`
in device resume callbacks remains unsupported when the low-power state stopped the timer.
