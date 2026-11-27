::area:: NXP Platform Drivers
::pr:: 110228

::migration-guide::
The i.MX RT118x boards now include a single part-core composer file
``nxp_rt118<part>_cm<core>.dtsi`` instead of the series-core file plus
a separate part overlay. Out-of-tree boards must update their devicetree
includes accordingly.

Example for a part that previously needed the series file plus a part
overlay:

.. code-block:: dts

  /* Before */
  #include <nxp/imxrt/nxp_rt118x_cm7.dtsi>
  #include <nxp/imxrt/nxp_rt1186.dtsi>

  /* After */
  #include <nxp/imxrt/imxrt118x/nxp_rt1186_cm7.dtsi>
