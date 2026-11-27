::area:: West project: hal_nxp
::pr:: 120694

::migration-guide::
The ``CONFIG_LV_USE_GPU_NXP_PXP`` Kconfig option has been removed. It only enabled the NXP PXP
HAL driver component, which is already enabled by :kconfig:option:`CONFIG_MCUX_PXP`, defaulting
to ``y`` when the :dtcompatible:`nxp,pxp` devicetree node is enabled and
:kconfig:option:`CONFIG_DISPLAY` is set. Applications that use the PXP driver component
without the display subsystem must enable :kconfig:option:`CONFIG_MCUX_PXP` explicitly.
