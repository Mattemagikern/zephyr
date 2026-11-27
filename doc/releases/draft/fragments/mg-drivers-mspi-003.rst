::area:: Drivers: MSPI
::pr:: 104657

::migration-guide::
The MSPI memory mapping feature has been renamed from "XIP" to "MEMMAP",
since XIP (:kconfig:option:`CONFIG_XIP`) is a software configuration concept
in Zephyr while the MSPI feature only memory-maps the device, which can be
used for data access as well as code execution. The MSPI
API is experimental, so no deprecated aliases are provided. Out-of-tree
users must update:

* ``CONFIG_MSPI_XIP`` -> :kconfig:option:`CONFIG_MSPI_MEMMAP`
* ``CONFIG_FLASH_MSPI_XIP_READ`` -> :kconfig:option:`CONFIG_FLASH_MSPI_MEMMAP_READ`
* ``struct mspi_xip_cfg`` -> ``struct mspi_memmap_cfg``
* ``enum mspi_xip_permit`` -> ``enum mspi_memmap_permit`` and its values
  ``MSPI_XIP_READ_WRITE``/``MSPI_XIP_READ_ONLY`` ->
  ``MSPI_MEMMAP_READ_WRITE``/``MSPI_MEMMAP_READ_ONLY``
* ``mspi_xip_config`` -> :c:func:`mspi_memmap_config` and the
  ``xip_config`` driver API entry -> ``memmap_config``
* ``MSPI_XIP_CONFIG_DT``/``MSPI_XIP_CONFIG_DT_INST``/``MSPI_XIP_CONFIG_DT_NO_CHECK``
  -> ``MSPI_MEMMAP_CONFIG_DT``/``MSPI_MEMMAP_CONFIG_DT_INST``/``MSPI_MEMMAP_CONFIG_DT_NO_CHECK``
* ``MSPI_XIP_CFG_STRUCT_DECLARE``/``MSPI_XIP_BASE_ADDR_DECLARE``/``MSPI_XIP_BASE_ADDR_INIT``
  -> ``MSPI_MEMMAP_CFG_STRUCT_DECLARE``/``MSPI_MEMMAP_BASE_ADDR_DECLARE``/``MSPI_MEMMAP_BASE_ADDR_INIT``
* devicetree property ``xip-config`` -> ``memmap-config`` on MSPI device nodes
