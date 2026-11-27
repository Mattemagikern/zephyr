::area:: MCU Manager
::pr:: 116745

::migration-guide::
``CONFIG_MCUMGR_GRP_OS_INFO_HARDWARE_INFO_SHORT_HARDWARE_PLATFORM`` has been removed. The
:ref:`mcumgr_os_application_info` command now always reports the board target as hardware
platform; the pre-4.3 board and board revision output is no longer available.
