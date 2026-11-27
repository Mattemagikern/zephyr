::area:: Drivers: System timer
::pr:: 115844

::migration-guide::
The ``bool idle`` argument of :c:func:`sys_clock_set_timeout` is deprecated. The
kernel now calls the new :c:func:`sys_clock_idle_enter` hook instead of
:c:func:`sys_clock_set_timeout` with ``idle=true``. For backwards compatibility, the
default implementation of :c:func:`sys_clock_idle_enter` emulates the old behavior by
calling :c:func:`sys_clock_set_timeout` with ``idle=true``, so timer drivers that
still examine ``idle`` keep working unchanged. Such drivers should be updated to
implement :c:func:`sys_clock_idle_enter` and move their ``idle``-specific handling
there. The argument is removed in a future release.

When :kconfig:option:`CONFIG_SYSTEM_CLOCK_SLOPPY_IDLE` is enabled, the kernel now
calls the new :c:func:`sys_clock_no_timeout` hook when no timeout is pending, instead
of :c:func:`sys_clock_set_timeout` with ``ticks=K_TICKS_FOREVER``. For backwards
compatibility, the default implementation of :c:func:`sys_clock_no_timeout` calls
:c:func:`sys_clock_set_timeout` with ``ticks=UINT32_MAX``, numerically the same
value, so drivers expecting it as the "no deadline" signal keep working unchanged,
whether they stop their clock or program a maximal wait. Such drivers should be
updated to implement :c:func:`sys_clock_no_timeout` instead.

Note the split between the two new hooks. Masking the wakeups while the CPU still
runs belongs in :c:func:`sys_clock_no_timeout`, which must leave
:c:func:`sys_clock_cycle_get_32` working. Stopping the time base belongs in
:c:func:`sys_clock_idle_enter` when it is passed ``SYS_CLOCK_IDLE_FOREVER``.
Refer to the :ref:`system timer driver documentation <system_timer_drivers>`
for the precise semantics.

Tickless system-timer drivers should no longer carry their own tick handling.
The implementation header :file:`drivers/timer/system_timer_generic.h` now
owns the accounting that each driver used to reimplement by hand, and got
subtly wrong: the cycle-to-tick conversion, the announce baseline, the
tick-aligned deadline computation and the counter range clamp, including the
wrap handling of a narrow counter. A driver reduces to a few cycle-domain
primitives, a cycle-counter read plus an absolute-compare arm, and includes
the header once, which emits :c:func:`sys_clock_set_timeout`,
:c:func:`sys_clock_elapsed` and :c:func:`sys_clock_cycle_get_32` /
:c:func:`sys_clock_cycle_get_64`.
