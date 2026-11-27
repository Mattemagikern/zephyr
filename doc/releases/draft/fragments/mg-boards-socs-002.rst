::area:: Boards/SoCs
::pr:: 114751

::migration-guide::
On RP2040 and RP2350, the ``vreg`` node (:dtcompatible:`raspberrypi,core-supply-regulator`) is
now ``disabled`` by default instead of ``okay``. Out-of-tree boards that need this regulator
must set ``status = "okay"`` on the ``&vreg`` node.

On RP2040, the ``regulator-always-on`` and ``regulator-allowed-modes =
<REGULATOR_RPI_PICO_MODE_NORMAL>`` properties are now set by default in the SoC dtsi. Boards
that previously set them explicitly can remove those lines.
