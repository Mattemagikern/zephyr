::area:: Kernel
::pr:: 116283

::migration-guide::
:c:func:`k_sleep` and :c:func:`k_usleep` are no longer system calls of their
own.  They are now inline wrappers around the new :c:func:`k_sleep_ticks`
system call, so that the compiler can fold or discard their unit conversions.
Their prototypes, semantics and return values are unchanged, and they have
moved from :file:`include/zephyr/kernel.h` to a new
:file:`include/zephyr/sleep.h` which :file:`kernel.h` includes.  Code calling
them needs no change.  Out of tree code taking their address, or relying on
``K_SYSCALL_K_SLEEP`` or ``K_SYSCALL_K_USLEEP``, must be updated to use
:c:func:`k_sleep_ticks` instead.  Note that it reports an early wakeup from
``K_FOREVER`` as ``K_TICKS_FOREVER``, where :c:func:`k_sleep` returns ``-1``.

The ``sys_port_trace_k_thread_sleep_*()``, ``sys_port_trace_k_thread_msleep_*()``
and ``sys_port_trace_k_thread_usleep_*()`` hooks are replaced by
``sys_port_trace_k_thread_sleep_ticks_enter()`` and
``sys_port_trace_k_thread_sleep_ticks_exit()``, since :c:func:`k_sleep_ticks`
is now the only one of the four that is not inline.  The exit hook reports
the time left to sleep in ticks, so a backend that presented it as
milliseconds should convert, for instance with
:c:func:`k_ticks_to_ms_ceil64`.  Out of tree tracing backends defining any of
the retired hooks must be updated.
