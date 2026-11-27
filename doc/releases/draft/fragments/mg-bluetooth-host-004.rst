::area:: Bluetooth Host
::pr:: 93033

::migration-guide::
Selected Bluetooth Host work items now run on the dedicated Bluetooth RX
workqueue instead of the system workqueue. Application callbacks reached from
those work items consequently run in the Bluetooth RX thread. This includes
connection teardown and deferred connection work (e.g. the ``disconnected()``
connection callback and SMP pairing timeouts), as well as timeout and
completion paths in ATT/GATT, L2CAP, AVDTP, and HFP AG.
Applications that relied on those callbacks running in the system workqueue
should review their synchronization and callback stack requirements. See
pull request :github:`93033` for details.
