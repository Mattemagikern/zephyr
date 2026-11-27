::area:: Boards/SoCs
::pr:: 116657, 116750

::migration-guide::
The following board name aliases, deprecated in v4.3 or earlier, have been removed. Build for the board target the alias used to
redirect to instead:

* ``arduino_uno_r4_minima`` → ``arduino_uno_r4@minima``
* ``arduino_uno_r4_wifi`` → ``arduino_uno_r4@wifi``
* ``esp32c6_devkitc`` → ``esp32c6_devkitc/esp32c6/hpcore``
* ``esp32_devkitc_wroom/esp32/procpu`` and ``esp32_devkitc_wrover/esp32/procpu`` →
  ``esp32_devkitc/esp32/procpu``
* ``esp32_devkitc_wroom/esp32/appcpu`` and ``esp32_devkitc_wrover/esp32/appcpu`` →
  ``esp32_devkitc/esp32/appcpu``
* ``neorv32`` → ``neorv32/neorv32/up5kdemo``
* ``panb511evb`` → ``panb611evb``
* ``raytac_an54l15q_db/nrf54l15/cpuapp`` → ``raytac_an54lq_db_15/nrf54l15/cpuapp``
* ``scobc_module1`` → ``scobc_a1``
* ``xiao_esp32c6`` → ``xiao_esp32c6/esp32c6/hpcore``
