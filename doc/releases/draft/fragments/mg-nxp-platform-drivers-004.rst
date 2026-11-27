::area:: NXP Platform Drivers
::pr:: 115593

::migration-guide::
The NXP Kinetis DTSI files were reorganized from the flat directory
``dts/arm/nxp/kinetis/`` into per-series subdirectories. Out-of-tree
boards that include these files directly must update their includes.

The new subdirectory layout is:

========================  ========================================
Kinetis series            New location
========================  ========================================
K2X                       ``dts/arm/nxp/kinetis/k2x/``
K32Lx                     ``dts/arm/nxp/kinetis/k32lx/``
K6X                       ``dts/arm/nxp/kinetis/k6x/``
K8X                       ``dts/arm/nxp/kinetis/k8x/``
KE1xF                     ``dts/arm/nxp/kinetis/ke1xf/``
KE1xZ                     ``dts/arm/nxp/kinetis/ke1xz/``
KL2X                      ``dts/arm/nxp/kinetis/kl2x/``
KV5X                      ``dts/arm/nxp/kinetis/kv5x/``
KWX                       ``dts/arm/nxp/kinetis/kwx/``
========================  ========================================

Example:

.. code-block:: dts

  /* Before */
  #include <nxp/kinetis/nxp_k66.dtsi>

  /* After */
  #include <nxp/kinetis/k6x/nxp_k66.dtsi>

The NXP MCX DTSI files were reorganized from the flat directory
``dts/arm/nxp/mcx/`` into per-series subdirectories. Out-of-tree boards
that include these files directly must update their includes.

The new subdirectory layout is:

========================  ========================================
MCX series                New location
========================  ========================================
MCXA                      ``dts/arm/nxp/mcx/mcxa/``
MCXC                      ``dts/arm/nxp/mcx/mcxc/``
MCXE                      ``dts/arm/nxp/mcx/mcxe/``
MCXL                      ``dts/arm/nxp/mcx/mcxl/``
MCXN                      ``dts/arm/nxp/mcx/mcxn/``
MCXW                      ``dts/arm/nxp/mcx/mcxw/``
========================  ========================================

Example:

.. code-block:: dts

  /* Before */
  #include <nxp/mcx/nxp_mcxc242.dtsi>

  /* After */
  #include <nxp/mcx/mcxc/nxp_mcxc242.dtsi>
