::area:: Drivers: System timer
::pr:: 111022

::migration-guide::
:c:func:`sys_clock_set_timeout`, :c:func:`sys_clock_announce` and
:c:func:`sys_clock_announce_locked` now take their tick count as an unsigned
``uint32_t`` rather than a signed ``int32_t``. Out-of-tree system timer drivers must
update their :c:func:`sys_clock_set_timeout` definition accordingly, otherwise the build
fails with a conflicting-types error. The kernel now also caps the requested timeout at
``SYS_CLOCK_MAX_WAIT`` and no longer passes ``K_TICKS_FOREVER`` to the driver, so such
drivers no longer need to clamp the request against the :c:func:`sys_clock_announce`
range or special-case ``K_TICKS_FOREVER``; only their own hardware cycle-count limits
still need enforcing.
