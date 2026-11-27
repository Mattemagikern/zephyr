::area:: Drivers: Clock control
::pr:: 116741

::migration-guide::
The Nordic Kconfig option ``CONFIG_NRFS_LOCAL_DOMAIN_DVFS_SCALE_DOWN_AFTER_INIT`` has been
removed. Use :kconfig:option:`CONFIG_CLOCK_CONTROL_NRF_HSFLL_LOCAL_REQ_LOW_FREQ` instead.
