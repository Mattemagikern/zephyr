::area:: Boards/SoCs
::pr:: 115905

::migration-guide::
The ``mimxrt1170_evk`` and ``mimxrt1160_evk`` cm7 targets and ``frdm_imxrt1152`` now ship a static
Arm MPU region table and enable :kconfig:option:`CONFIG_ARM_MPU_CM7_UNMAPPED_REGION` by default.
The MPU therefore no longer falls back to the ``PRIVDEFENA`` background map, and addresses outside
that table are no longer reachable. The table covers ITCM, DTCM, the CM4-shared OCRAM image region
on the dual-core boards, the RAM backing the ``zephyr,sram`` chosen node, the FlexSPI NOR window
and the peripheral aperture, but not the remaining on-chip banks: ``ocram1`` and ``ocram2`` on
``mimxrt1170_evk`` and ``frdm_imxrt1152``, and ``ocram_combined`` on ``mimxrt1160_evk``.

Applications that place data in one of those banks must claim it explicitly with a
``zephyr,memory-attr`` node, which is then given an MPU region of its own, as the in-tree
:file:`samples/subsys/ipc` samples do for the memory they share with the CM4. Setting
:kconfig:option:`CONFIG_ARM_MPU_CM7_UNMAPPED_REGION` to ``n`` restores the previous behavior.
