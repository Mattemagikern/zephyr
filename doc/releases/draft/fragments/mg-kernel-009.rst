::area:: Kernel
::pr:: 119031

::migration-guide::
The :ref:`object core framework <object_cores_api>` no longer links objects
into per-type lists through a node in the object. Statically defined objects
are enumerated in place and objects initialized at run time are referenced
from a bounded registry sized by
:kconfig:option:`CONFIG_OBJ_CORE_MAX_DYNAMIC_OBJECTS`. The ``node`` member of
:c:struct:`k_obj_core` and the ``list`` member of :c:struct:`k_obj_type` are
removed; tools that walked those lists must use
:c:func:`k_obj_type_walk_locked` or :c:func:`k_obj_type_walk_unlocked`.
:c:macro:`K_OBJ_TYPE_DEFINE` now defines the :c:struct:`k_obj_type` variable
itself, so a separate declaration of that variable must be dropped. Objects
located in a thread's stack or in the interrupt stack are no longer
registered, and :kconfig:option:`CONFIG_OBJ_CORE` selects
:kconfig:option:`CONFIG_THREAD_STACK_INFO`, which adds the stack information
fields to every :c:struct:`k_thread`. The new
:kconfig:option:`CONFIG_OBJ_CORE_QUEUE`, enabled by default, adds an object
core to every :c:struct:`k_queue`; as FIFOs and LIFOs embed a queue, they are
reported by both their own object type and the queue type.

Object tracking (:kconfig:option:`CONFIG_TRACING_OBJECT_TRACKING`) is now
provided by the object core framework, which it selects. The
``_track_list_k_*`` list heads, the ``SYS_PORT_TRACK_NEXT()`` macro, the
``sys_track_*_init()`` hooks and the :file:`include/zephyr/tracing/tracking.h`
header are removed. Code that walked the tracking lists must use
:c:func:`k_obj_type_walk_locked` or :c:func:`k_obj_type_walk_unlocked` with
the object type found by :c:func:`k_obj_type_find`.
