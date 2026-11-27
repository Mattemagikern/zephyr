::area:: NXP Platform Drivers
::pr:: 119001

::migration-guide::
The NXP MCXN series gained dedicated per-part composer DTSI files for
mcxn547, mcxn947 and mcxn236 (``nxp_mcxn547.dtsi``, ``nxp_mcxn947.dtsi``
and ``nxp_mcxn236.dtsi``), alongside the new mcxn546, mcxn946 and mcxn235
phantom parts added this release. Each of these files just includes the
existing series file (``nxp_mcxn54x.dtsi``, ``nxp_mcxn94x.dtsi`` and
``nxp_mcxn23x.dtsi`` respectively) with no overrides, and in-tree boards
for mcxn547, mcxn947 and mcxn236 now include the new per-part file
instead. The series files themselves are unchanged and still work if
included directly, so this is not a required migration, but out-of-tree
boards for these three parts may want to switch to the new per-part
files for consistency with the rest of the series.

Example:

.. code-block:: dts

  /* Before */
  #include <nxp/mcx/mcxn/nxp_mcxn94x.dtsi>

  /* After */
  #include <nxp/mcx/mcxn/nxp_mcxn947.dtsi>
