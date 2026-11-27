::area:: Boards/SoCs
::pr:: 117192

::migration-guide::
On RP2350 (rpi_pico family), the ``hazard3`` and ``m33`` cpucluster qualifiers are deprecated in
favor of ``hazard3_0`` and ``m33_0``, which explicitly identify the cluster as CPU0 and pave the
way for dual-core support. All in-tree RP2350 boards have been migrated to the new qualifiers
(e.g. ``rpi_pico2/rp2350a/m33`` to ``rpi_pico2/rp2350a/m33_0``). Out-of-tree boards using the bare
``hazard3``/``m33`` qualifiers should rename their board files, ``board.yml`` ``cpucluster:``
entries, and Kconfig select lines to use ``SOC_RP2350[AB]_HAZARD3_0``/``SOC_RP2350[AB]_M33_0``.
The bare ``hazard3``/``m33`` entries in ``soc.yml`` and the corresponding
``SOC_RP2350[AB]_HAZARD3``/``_M33`` Kconfig symbols are deprecated and will both be removed in a
future release.
