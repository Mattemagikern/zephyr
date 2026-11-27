::area:: Drivers: CAN
::pr:: 110064

::migration-guide::
The header files for the NXP SJA1000 (``can_sja1000.h``) and Bosch M_CAN (``can_mcan.h``) CAN
controller driver backends where converted to library-specific includes. Out-of-tree drivers based
on these backends will need to update their include directives accordingly.
