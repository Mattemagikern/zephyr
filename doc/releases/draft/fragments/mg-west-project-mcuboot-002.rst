::area:: West project: mcuboot
::pr:: 118393

::migration-guide::
Sysbuild no longer forces the MCUboot overwrite-only mode and unsigned images on Espressif
SoCs. Boards using them now get the generic defaults: swap using offset, which keeps the
previous image for a revert, and RSA-2048 signatures with the MCUboot development key.
Projects with their own key must set :kconfig:option:`SB_CONFIG_BOOT_SIGNATURE_KEY_FILE`,
and projects that relied on the previous behavior can select
:kconfig:option:`SB_CONFIG_MCUBOOT_MODE_OVERWRITE_ONLY` and
:kconfig:option:`SB_CONFIG_BOOT_SIGNATURE_TYPE_NONE` explicitly. A bootloader built after
this change rejects unsigned images, so the bootloader and the application must be
reflashed together when a device is moved to the new defaults.

The shared Espressif partition tables no longer define a ``scratch_partition``, so
:kconfig:option:`SB_CONFIG_MCUBOOT_MODE_SWAP_SCRATCH` is no longer available on boards using
them. The other partitions keep their offsets. Projects that need it can add the partition
back in a board overlay.
