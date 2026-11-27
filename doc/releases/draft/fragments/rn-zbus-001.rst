::area:: zbus
::pr:: 113138

::release-notes::
:kconfig:option:`CONFIG_ZBUS_MSG_SUBSCRIBER_NET_BUF_POOL_ISOLATION` now works without requiring
a dedicated pool on every channel (channels fall back to the shared pool until
:c:func:`zbus_chan_set_msg_sub_pool` is called)
