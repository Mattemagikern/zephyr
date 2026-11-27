::area:: Boards/SoCs
::pr:: 110797

::migration-guide::
The default MCUboot signature type for the boards stm32h573i_dk and b_u585i_iot02a has
been changed from RSA-3072 to EC-P256. This affects builds that have MCUboot enabled in
TF-M (:kconfig:option:`CONFIG_TFM_BL2`). If you wish to keep using RSA-3072, you need
to set :kconfig:option:`CONFIG_TFM_MCUBOOT_SIGNATURE_TYPE` to ``"RSA-3072"``.
Otherwise, make sure to have your own signing keys of the signature type in use.
