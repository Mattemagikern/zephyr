::area:: Bluetooth Mesh
::pr:: 116743

::migration-guide::
The deprecated ``CONFIG_BT_MESH_BLOB_IO_FLASH_WITH_ERASE`` and
``CONFIG_BT_MESH_BLOB_IO_FLASH_WITHOUT_ERASE`` Kconfig options have been removed, with no
replacement. They have been deprecated since Zephyr 4.3, where the BLOB IO Flash module
started querying the erase capability at runtime, and have had no effect since.
