::area:: Networking: Wi-Fi
::pr:: 122222

::release-notes::
``NET_REQUEST_WIFI_CONN_PARAMS`` no longer returns the caller's buffers of the
last connect request. The ``psk``, ``sae_password`` and EAP fields are now NULL,
and the SSID stays valid until the next connect request.
