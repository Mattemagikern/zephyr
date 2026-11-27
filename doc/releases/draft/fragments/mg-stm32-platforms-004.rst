::area:: STM32 Platforms
::pr:: 104423

::migration-guide::
:dtcompatible:`st,stm32-sai` binding has been restructured to reflect the SAI hardware
topology. The parent node now represents the SAI Block controller, while a new
``child-binding`` represents the SAI Sub-Block instances.
The following properties shall be moved from the parent SAI node to a child sub-block node:
``dmas``, ``dma-names`` (now validated against ``enum: [tx, rx]``), ``pinctrl-0``,
``pinctrl-names``, ``mclk-enable``, ``mclk-divider``, ``synchronous``, and
``fifo-threshold``.
