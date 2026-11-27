::area:: Drivers: Stepper
::pr:: 110062

::migration-guide::
The ``activate-stallguard2``, ``stallguard-threshold-velocity`` and ``stallguard-velocity-check-interval-ms``
properties of :dtcompatible:`adi,tmc50xx-stepper-ctrl` and :dtcompatible:`adi,tmc51xx-stepper-ctrl` have
been removed. The stallguard configuration is now done at runtime using
:c:func:`tmc50xx_stepper_ctrl_configure_stallguard`, :c:func:`tmc51xx_stepper_ctrl_configure_stallguard`
and :c:struct:`tmc_stallguard_settings`. Out-of-tree drivers using these properties
must be updated to remove them.
