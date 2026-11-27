::area:: Networking: Wi-Fi
::pr:: 114962

::release-notes::
P2P gained shell commands to set the local device name and to query the
current P2P status, backed by the new :c:enumerator:`WIFI_P2P_SET_DEV_NAME`
and :c:enumerator:`WIFI_P2P_STATUS` :c:enum:`wifi_p2p_op` operations. The
corresponding ``device_name`` and ``status`` members were added to
:c:struct:`wifi_p2p_params`, along with the new
:c:macro:`WIFI_P2P_STATUS_BUF_SIZE` buffer-size macro.
