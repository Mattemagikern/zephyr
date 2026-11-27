::area:: Bluetooth Host
::pr:: 112009

::migration-guide::
The ``le_param_updated`` callback in :c:struct:`bt_conn_cb` is no longer invoked when a
connection parameter update fails (i.e. the LE Connection Update Complete event reports a
non-zero status). Previously it was called unconditionally, reporting the unchanged
connection parameters with no error indication, which could not be distinguished from a
successful update. Applications that need to be notified about rejected, application-initiated
parameter updates should enable :kconfig:option:`CONFIG_BT_USER_CONN_PARAM_REJECTED` and
implement the new ``le_param_update_rejected`` callback.
