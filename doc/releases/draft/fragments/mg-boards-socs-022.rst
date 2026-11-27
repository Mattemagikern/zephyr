::area:: Boards/SoCs
::pr:: 114172

::migration-guide::
Boards must now select :kconfig:option:`CONFIG_TFM_PARTITION_FIRMWARE_UPDATE_SUPPORTED` if they
support firmware update via TF-M.
