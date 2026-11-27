::area:: NXP Platform Drivers
::pr:: 106149

::migration-guide::
Kinetis KE1xF no longer requires a board overlay to designate the system
timer when :kconfig:option:`CONFIG_PM` is enabled. The SoC DTSI now sets the
``zephyr,system-timer`` chosen property, so boards that added the overlay
described in the Zephyr 4.4 migration guide can remove it.
