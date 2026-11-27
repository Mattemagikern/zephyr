::area:: Drivers: System timer
::pr:: 115844

::release-notes::
With :kconfig:option:`CONFIG_SYSTEM_CLOCK_SLOPPY_IDLE` enabled, a driver may no
longer stop its time base as soon as no timeout is pending, if that breaks
:c:func:`sys_clock_cycle_get_32` / :c:func:`sys_clock_cycle_get_64`. Those must
keep counting while the CPU runs. Stopping the time base is permitted only from
:c:func:`sys_clock_idle_enter`, where :c:func:`sys_clock_idle_exit` is
guaranteed to follow.

Tickless system-timer drivers can now be built on a shared implementation
header, :file:`drivers/timer/system_timer_generic.h`, which owns the tick
accounting each driver previously open-coded: the cycle-to-tick conversion,
the announce baseline, the tick-aligned deadline and the counter wrap and
range handling. A driver reduces to a few cycle-domain primitives, a
cycle-counter read plus an absolute-compare arm. See the
:ref:`migration guide <migration_4.5>` for how to use it.
