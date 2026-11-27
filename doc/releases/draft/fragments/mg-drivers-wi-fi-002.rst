::area:: Drivers: Wi-Fi
::pr:: 109321

::migration-guide::
The Espressif Wi-Fi driver Kconfig option ``CONFIG_ESP32_WIFI_STA_AUTO_DHCPV4`` has been
removed in favor of the generic :kconfig:option:`CONFIG_WIFI_STA_AUTO_DHCPV4`. Applications
that previously disabled the Espressif-specific option must now disable the generic option
to retain manual DHCPv4 or static IP behavior after STA connection.
