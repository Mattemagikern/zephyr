::area:: Networking
::pr:: 115654

::migration-guide::
:kconfig:option:`CONFIG_OPENTHREAD_JOINER_PSKD` no longer defaults to the publicly
documented ``"J01NME"`` credential, and now has no default at all. Builds that enable
:kconfig:option:`CONFIG_OPENTHREAD_JOINER_AUTOSTART` fail until a PSKd of 6 to 32
uppercase alphanumeric characters (0-9 and A-Z excluding I, O, Q and Z) is configured.

A PSKd set through Kconfig is compiled into the image and is therefore identical on
every unit running that firmware, which lets any commissioner in range enrol an
uncommissioned device into its own network. Treat the option as a development aid:
product firmware should read a per-device PSKd from factory data and call
``otJoinerStart()`` from the application instead.
