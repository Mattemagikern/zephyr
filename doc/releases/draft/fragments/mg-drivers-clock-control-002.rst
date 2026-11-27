::area:: Drivers: Clock control
::pr:: 106881

::migration-guide::
The :dtcompatible:`nxp,imxrt11xx-arm-pll` binding now uses ``loop-div`` and
``post-div`` for ARM PLL configuration. The legacy ``clock-mult`` and
``clock-div`` properties remain supported but are deprecated. Existing
RT11xx overlays should be updated using the mapping
``loop-div = clock-mult * 2`` and ``post-div = clock-div``.
