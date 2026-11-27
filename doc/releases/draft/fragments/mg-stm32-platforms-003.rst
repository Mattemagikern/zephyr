::area:: STM32 Platforms
::pr:: 106188

::migration-guide::
SoC DTSI files now consistently use interrupt priority zero for all peripherals.
Applications must now explicitly configure interrupt priorities using Devicetree
if they previously relied on the values found in SoC DTSI files.
