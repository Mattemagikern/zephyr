::area:: Drivers: Wi-Fi
::pr:: 116704

::migration-guide::
:c:struct:`wifi_status` gained ``status_code`` and ``reason_code`` members carrying the raw
IEEE 802.11 codes, so the struct is larger than the ``int`` it used to be. Code that raises
or receives :c:enumerator:`NET_EVENT_WIFI_SCAN_DONE`,
:c:enumerator:`NET_EVENT_WIFI_CONNECT_RESULT`,
:c:enumerator:`NET_EVENT_WIFI_DISCONNECT_RESULT`,
:c:enumerator:`NET_EVENT_WIFI_DISCONNECT_COMPLETE`,
:c:enumerator:`NET_EVENT_WIFI_AP_ENABLE_RESULT` or
:c:enumerator:`NET_EVENT_WIFI_AP_DISABLE_RESULT` must use ``sizeof(struct wifi_status)``
rather than ``sizeof(int)`` for the event payload length. Event handlers that only read the
status value are unaffected, as it remains the first member.

A Wi-Fi connect request that runs past its timeout now reports
:c:enumerator:`WIFI_STATUS_CONN_TIMEOUT` in :c:enumerator:`NET_EVENT_WIFI_CONNECT_RESULT`
instead of a raw ``-ETIMEDOUT``. This only concerns the connections handled by the
supplicant. Applications that matched on the errno value have to match on the status
value instead.
