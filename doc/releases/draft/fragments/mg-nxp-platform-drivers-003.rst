::area:: NXP Platform Drivers
::pr:: 115524

::migration-guide::
The NXP LPC DTSI files were reorganized from the flat directory
``dts/arm/nxp/lpc/`` into per-series subdirectories. Out-of-tree boards that
include these files directly must update their includes.

The new subdirectory layout is:

========================  ========================================
LPC series                New location
========================  ========================================
LPC11U6x                  ``dts/arm/nxp/lpc/lpc11u6x/``
LPC51U68                  ``dts/arm/nxp/lpc/lpc51u68/``
LPC54xxx                  ``dts/arm/nxp/lpc/lpc54xxx/``
LPC55xxx                  ``dts/arm/nxp/lpc/lpc55xxx/``
LPC84x                    ``dts/arm/nxp/lpc/lpc84x/``
========================  ========================================

Example:

.. code-block:: dts

  /* Before */
  #include <nxp/lpc/nxp_lpc55S6x.dtsi>

  /* After */
  #include <nxp/lpc/lpc55xxx/nxp_lpc55S6x.dtsi>
