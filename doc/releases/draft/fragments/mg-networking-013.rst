::area:: Networking
::pr:: 111554

::migration-guide::
The default WPA supplicant network selection criterion has changed from
throughput-based to reliability-based (SNR), switching the
:kconfig:option:`CONFIG_WIFI_NM_WPA_SUPPLICANT_NW_SEL` Kconfig default from
:kconfig:option:`CONFIG_WIFI_NM_WPA_SUPPLICANT_NW_SEL_THROUGHPUT` to
:kconfig:option:`CONFIG_WIFI_NM_WPA_SUPPLICANT_NW_SEL_RELIABILITY`.
Previously, SNR above 25 dBm was considered sufficient and largely excluded
from AP selection; SNR is now always factored in, improving connection stability
for embedded Wi-Fi use cases. Users who need the previous behaviour can restore it by enabling
:kconfig:option:`CONFIG_WIFI_NM_WPA_SUPPLICANT_NW_SEL_THROUGHPUT`.
