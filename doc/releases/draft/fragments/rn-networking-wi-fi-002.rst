::area:: Networking: Wi-Fi
::pr:: 116704

::release-notes::
The connection result event can now say that the access point rejected the
authentication or the association, through the new
:c:enumerator:`WIFI_STATUS_CONN_AUTH_REJECT` and
:c:enumerator:`WIFI_STATUS_CONN_ASSOC_REJECT` values, and
:c:struct:`wifi_status` carries the raw IEEE 802.11 status and reason codes
behind the failure. The supplicant fills these in, and the Wi-Fi shell prints
them with the connection and disconnection results.
