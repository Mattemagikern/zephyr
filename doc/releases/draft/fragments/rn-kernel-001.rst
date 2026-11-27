::area:: Kernel
::pr:: 119031

::release-notes::
The :ref:`object core framework <object_cores_api>` no longer keeps registry state inside
the objects it tracks. Statically defined objects are enumerated in place and objects
initialized at run time are referenced from a bounded registry, so a kernel object may be
declared on a stack, embedded in freed memory or initialized again without corrupting the
registry. Objects in stack storage are not tracked.
