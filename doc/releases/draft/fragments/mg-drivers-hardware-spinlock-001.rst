::area:: Drivers: Hardware Spinlock
::pr:: 114730

::migration-guide::
The ``num-locks`` DeviceTree property is now a standard, required property of the hwspinlock
controller binding. Every hwspinlock controller node must set it, and out-of-tree bindings
must drop their own ``num-locks`` ``type``/``required`` declarations.

:c:func:`hw_spin_lock`, :c:func:`hw_spin_trylock` and :c:func:`hw_spin_unlock` no longer take a
``hwspinlock_ctx_t *`` argument; the per-lock Zephyr spinlock now lives in the driver's config.
``struct hwspinlock_context``, ``hwspinlock_ctx_t``, the ``ctx`` member of
:c:struct:`hwspinlock_dt_spec` and ``HWSPINLOCK_CTX_INITIALIZER`` have been removed. The
:c:func:`hw_spin_lock_dt`, :c:func:`hw_spin_trylock_dt` and :c:func:`hw_spin_unlock_dt`
helpers are unchanged. As a result, all :c:macro:`HWSPINLOCK_DT_SPEC_GET` instances referring to
the same hardware spinlock now share a single Zephyr spinlock, instead of each getting its own.

Hardware spinlock drivers must now embed :c:struct:`hwspinlock_driver_config` as the first member
of their config struct, initialize it with :c:macro:`HWSPINLOCK_COMMON_CONFIG_FROM_DT_INST` (or
:c:macro:`HWSPINLOCK_COMMON_CONFIG_FROM_DT_NODE`), and declare the backing spinlock array with
:c:macro:`HWSPINLOCK_SPINLOCK_ARRAY_DT_INST_DEFINE` (or
:c:macro:`HWSPINLOCK_SPINLOCK_ARRAY_DT_DEFINE`). The ``get_max_id`` driver operation is now
optional: when it is not implemented, :c:func:`hw_spinlock_get_max_id` default implementation
returns the value of the ``num-locks`` DeviceTree property minus 1.
