::area:: Networking: Ethernet
::pr:: 107326

::migration-guide::
The :kconfig:option:`CONFIG_ETH_QEMU_EXTRA_ARGS` and
:kconfig:option:`CONFIG_NET_QEMU_USER_EXTRA_ARGS` options can no longer be used to specify the MAC
address for the QEMU Ethernet device. Instead, :kconfig:option:`CONFIG_NET_QEMU_DEVICE_EXTRA_ARGS`
can be used. This is because we are no longer using the ``-nic`` option for QEMU, but the
``-netdev`` and ``-device`` options.
