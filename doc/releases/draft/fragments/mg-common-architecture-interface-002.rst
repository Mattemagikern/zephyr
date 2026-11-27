::area:: Common Architecture Interface
::pr:: 110037

::migration-guide::
``CONFIG_XTENSA_MPU_ONLY_SOC_RANGES`` is removed. For SoC or board to override the default
MPU region table, override :c:var:`xtensa_mpu_ranges` in the SoC or board layer instead.

``xtensa_soc_mpu_ranges[]`` and ``xtensa_soc_mpu_ranges_num`` are removed. If SoC or board
needs its own memory regions at boot, override :c:var:`xtensa_mpu_ranges` instead.
