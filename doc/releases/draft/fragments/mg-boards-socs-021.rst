::area:: Boards/SoCs
::pr:: 114670

::migration-guide::
The mimxrt1180_evk Kconfig option ``NXP_BOARD_SPECIFIC_MPU_SETTINGS`` has been renamed to
:kconfig:option:`CONFIG_BOARD_NXP_SPECIFIC_MPU_SETTINGS`, matching the ``BOARD_NXP_*`` naming
used by the other NXP board options and by frdm_imxrt1186. Configurations setting
``CONFIG_NXP_BOARD_SPECIFIC_MPU_SETTINGS`` must be updated to the new name.
