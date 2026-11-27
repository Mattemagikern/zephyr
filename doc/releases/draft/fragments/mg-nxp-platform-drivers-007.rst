::area:: NXP Platform Drivers
::pr:: 115593

::migration-guide::
The NXP i.MX RT DTSI files were reorganized from the flat directory
``dts/arm/nxp/imxrt/`` into per-series subdirectories, Out-of-tree
boards that include these files directly must update their includes.

The new subdirectory layout is:

========================  ========================================
i.MX RT series            New location
========================  ========================================
RT10xx                    ``dts/arm/nxp/imxrt/imxrt10xx/``
RT11xx                    ``dts/arm/nxp/imxrt/imxrt11xx/``
RT5xx                     ``dts/arm/nxp/imxrt/imxrt5xx/``
RT6xx                     ``dts/arm/nxp/imxrt/imxrt6xx/``
RT7xx                     ``dts/arm/nxp/imxrt/imxrt7xx/``
RT118x                    ``dts/arm/nxp/imxrt/imxrt118x/``
========================  ========================================

Example:

.. code-block:: dts

  /* Before */
  #include <nxp/imxrt/nxp_rt1060.dtsi>

  /* After */
  #include <nxp/imxrt/imxrt10xx/nxp_rt1060.dtsi>
