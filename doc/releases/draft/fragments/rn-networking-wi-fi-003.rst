::area:: Networking: Wi-Fi
::pr:: 118844

::release-notes::
The ESP32 Wi-Fi driver gained 802.11k/v/r and MBO support, all off by
default. :kconfig:option:`CONFIG_ESP32_WIFI_11KV_SUPPORT` turns on 802.11k
and 802.11v, :kconfig:option:`CONFIG_ESP32_WIFI_MBO_SUPPORT` turns on MBO
on top of them, and :kconfig:option:`CONFIG_ESP32_WIFI_11R_SUPPORT` turns
on 802.11r. The ``wifi 11k`` and ``wifi 11v_btm_query`` shell commands now
work on ESP32 while the station is associated, and
:kconfig:option:`CONFIG_ESP32_WIFI_SIGNAL_CHANGE_EVENT` raises
:c:macro:`NET_EVENT_WIFI_SIGNAL_CHANGE` when the signal of the connected
access point weakens, for an application that decides when to roam.
