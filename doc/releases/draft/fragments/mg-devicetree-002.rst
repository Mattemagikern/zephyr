::area:: Devicetree
::pr:: 116787

::migration-guide::
The ``zephyr,memory-region-mpu`` property has been removed. Use ``zephyr,memory-attr``
instead. It takes an integer bitmask, not a string:

.. code-block:: none

   "RAM"         -> <DT_MEM_ARM_MPU_RAM>
   "RAM_NOCACHE" -> <DT_MEM_ARM_MPU_RAM_NOCACHE>
   "FLASH"       -> <DT_MEM_ARM_MPU_FLASH>
   "PPB"         -> <DT_MEM_ARM_MPU_PPB>
   "IO"          -> <DT_MEM_ARM_MPU_IO>
   "EXTMEM"      -> <DT_MEM_ARM_MPU_EXTMEM>
