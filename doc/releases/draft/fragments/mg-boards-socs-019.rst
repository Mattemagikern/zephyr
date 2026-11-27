::area:: Boards/SoCs
::pr:: 111754

::migration-guide::
The clock configuration for all Silabs Series 0 and Series 1 boards needs to be specified in the
device tree now. The Kconfigs ``CONFIG_SOC_GECKO_HAS_HFRCO_FREQRANGE`` and ``CONFIG_CMU_*`` have
been removed. See :github:`111754` for examples of how to adapt your board.
