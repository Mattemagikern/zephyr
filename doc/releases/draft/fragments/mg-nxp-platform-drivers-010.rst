::area:: NXP Platform Drivers
::pr:: 116931

::migration-guide::
The NXP SoC pin control headers under the ``hal_nxp`` ``dts/nxp/`` tree were
reorganized to mirror the ``dts/arm/nxp/<family>/<series>/`` layout: every
SoC ``*-pinctrl.h`` / ``*-pinctrl.dtsi`` file moved into a ``pinctrl/``
subdirectory. Out-of-tree boards that include these SoC pin control headers
directly must update their includes.

The families that are series-organized (i.MX RT, Kinetis, LPC, MCX) place
their headers in ``<family>/<series>/pinctrl/``; the families that are flat
(i.MX, S32, RW) place theirs in a family-level ``<family>/pinctrl/``
directory. Kinetis additionally adds new series directories (``k0x``,
``km3x``, ``kv3x``) for parts that had none. In addition the former
``nxp_imx`` directory was split: ``nxp_imx/rt/`` became the ``imxrt/`` family
and the remaining i.MX application processors became the flat ``imx/`` family.

Examples:

.. code-block:: dts

  /* Before */
  #include <nxp/nxp_imx/rt/mimxrt1151dvm8b-pinctrl.dtsi>
  #include <nxp/nxp_imx/mimx8ml8dvnlz-pinctrl.dtsi>
  #include <nxp/kinetis/MK64FN1M0VLL12-pinctrl.h>

  /* After */
  #include <nxp/imxrt/imxrt11xx/pinctrl/mimxrt1151dvm8b-pinctrl.dtsi>
  #include <nxp/imx/pinctrl/mimx8ml8dvnlz-pinctrl.dtsi>
  #include <nxp/kinetis/k6x/pinctrl/MK64FN1M0VLL12-pinctrl.h>
