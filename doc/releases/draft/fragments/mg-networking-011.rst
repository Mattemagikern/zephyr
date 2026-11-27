::area:: Networking
::pr:: 111201

::migration-guide::
The number of ZVFS eventfd's is now determined by a ``ZVFS_EVENTFD_SIZE`` define
instead of using the :kconfig:option:`CONFIG_ZVFS_EVENTFD_MAX` Kconfig option directly.
Subsystems can specify their own eventfd count requirements by specifying Kconfig
options with the prefix ``CONFIG_ZVFS_EVENTFD_ADD_SIZE_``. These are summed together
and the result is compared against :kconfig:option:`CONFIG_ZVFS_EVENTFD_MAX`; the larger
of the two values is used. To force :kconfig:option:`CONFIG_ZVFS_EVENTFD_MAX` to be used,
even when its value is less than the sum of the custom requirements, a new
:kconfig:option:`CONFIG_ZVFS_EVENTFD_IGNORE_MIN` option has been introduced (which
defaults to being disabled). As a result, networking subsystems that allocate eventfds
(e.g. HTTP server, CoAP server, LwM2M, PTP, SSH, the socket service and the WPA
supplicant) no longer require the application to manually bump
:kconfig:option:`CONFIG_ZVFS_EVENTFD_MAX` to account for them.
