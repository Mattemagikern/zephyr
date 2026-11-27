::area:: Boards/SoCs
::pr:: 117461

::migration-guide::
On NXP LPC54xxx, ``CONFIG_LPC54XXX_SRAM2_CLOCK`` has been replaced by
``CONFIG_SOC_SERIES_LPC54XXX_SRAM_CLOCKS``. The old name fitted while the
LPC54114 was the only SoC in the series, where CMSIS ``SystemInit()`` enables
just SRAM2. On the LPC546xx it enables SRAM2 and SRAM3, so the option now
covers more than the one bank it was named after. Both default to ``y``. A
configuration that assigned the old symbol has to be updated, and fails to
build until it is.
