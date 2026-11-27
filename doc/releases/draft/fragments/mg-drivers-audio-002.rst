::area:: Drivers: Audio
::pr:: 107695

::migration-guide::
The DMIC driver backend API now uses :c:struct:`dmic_driver_api` instead of ``struct _dmic_ops``.

Out-of-tree DMIC drivers must rename their backend API struct definitions and switch their API
instances to ``DEVICE_API(dmic, ...)``. See :github:`107695` for examples of how in-tree drivers
have been updated. Application code using :c:func:`dmic_configure`, :c:func:`dmic_trigger`, and
:c:func:`dmic_read` is not impacted.
