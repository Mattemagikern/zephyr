::area:: Modem
::pr:: 111382

::migration-guide::
The Kconfig option :kconfig:option:`CONFIG_MODEM_SIMCOM_SIM7080_LTE_BANDS` has been split
into :kconfig:option:`CONFIG_MODEM_SIMCOM_SIM7080_LTE_BANDS_M1` and
:kconfig:option:`CONFIG_MODEM_SIMCOM_SIM7080_LTE_BANDS_NB1` since NB-IoT and CAT-M have
slightly different usable bands. The type of the newly introduced configuration values
is a hex bitmap of selected bands. By default bands 8, 20 and 28 are selected.

Applications configuring the :kconfig:option:`CONFIG_MODEM_SIMCOM_SIM7080_LTE_BANDS`
must update their configuration.
