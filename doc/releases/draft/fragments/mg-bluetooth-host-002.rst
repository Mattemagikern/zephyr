::area:: Bluetooth Host
::pr:: 93033

::migration-guide::
The ``CONFIG_BT_RECV_CONTEXT`` Kconfig choice and its options ``CONFIG_BT_RECV_WORKQ_SYS``
and ``CONFIG_BT_RECV_WORKQ_BT`` have been removed. The host now unconditionally
processes low-priority HCI packets on the dedicated Bluetooth RX workqueue (the
previous ``CONFIG_BT_RECV_WORKQ_BT`` behavior).
Applications that selected ``CONFIG_BT_RECV_WORKQ_SYS`` to save RAM (e.g. on
nRF51) must drop that option; the dedicated RX thread is now always created.
Tune :kconfig:option:`CONFIG_BT_RX_STACK_SIZE` (the RX thread stack) for the
application's enabled host features. Since low-priority RX no longer runs on
the system workqueue, applications may be able to reduce
:kconfig:option:`CONFIG_SYSTEM_WORKQUEUE_STACK_SIZE`, but both stack sizes are
application-specific and should be validated using stack-usage measurements.
