::area:: Bluetooth Host
::pr:: 119123

::release-notes::
:kconfig:option:`CONFIG_SYSTEM_WORKQUEUE_PRIORITY` is no longer forced to a
cooperative priority by :kconfig:option:`CONFIG_BT` alone. Only the components
that submit work to the system workqueue require it now, so a build without any
of them, such as an HCI raw image driving an external controller, can select a
preemptible priority again.
