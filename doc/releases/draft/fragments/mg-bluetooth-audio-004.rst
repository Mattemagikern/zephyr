::area:: Bluetooth Audio
::pr:: 107980

::migration-guide::
CSIP

* Optional CSIS characteristics have been made configurable via Kconfig and must be enabled
  explicitly:

  * Coordinated Set Size → :kconfig:option:`CONFIG_BT_CSIP_SET_MEMBER_SIZE_SUPPORT`
  * Set Member Lock → :kconfig:option:`CONFIG_BT_CSIP_SET_MEMBER_LOCK_SUPPORT`
  * Set Member Rank → :kconfig:option:`CONFIG_BT_CSIP_SET_MEMBER_RANK_SUPPORT`
