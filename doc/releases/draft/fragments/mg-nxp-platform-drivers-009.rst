::area:: NXP Platform Drivers
::pr:: 117796

::migration-guide::
The i.MX RT7xx boards now include a single part-core composer file
``nxp_<part>_<core>.dtsi`` instead of the series-core file. Out-of-tree
boards must update their devicetree includes accordingly.

Example for a part that previously needed the series file:

.. code-block:: dts

  /* Before */
  #include <nxp/imxrt/imxrt7xx/nxp_rt7xx_cm33_cpu0.dtsi>

  /* After */
  #include <nxp/imxrt/imxrt7xx/nxp_rt798s_cm33_cpu0.dtsi>
