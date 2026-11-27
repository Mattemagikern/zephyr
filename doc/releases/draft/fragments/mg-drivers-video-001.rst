::area:: Drivers: Video
::pr:: 109393

::migration-guide::
The :dtcompatible:`ovti,ov7670` and :dtcompatible:`ovti,ov7675` camera drivers now assume a
24 MHz XCLK input instead of the previous 6 MHz, matching the typical XCLK frequency listed in
the OV7670 datasheet. Boards driving an OV7670 or OV7675 sensor must update their board-level
XCLK clock configuration accordingly. For example, ``frdm_mcxn236`` has been switched from
``kFRO12M_to_CLKOUT`` (divided by 2 to yield 6 MHz) to ``kFRO_HF_to_CLKOUT`` (divided by 2 to
yield 24 MHz), and ``frdm_mcxn947`` keeps ``kMAIN_CLK_to_CLKOUT`` but changes the CLKOUT
divider from 25 to 6 to yield 24 MHz.
