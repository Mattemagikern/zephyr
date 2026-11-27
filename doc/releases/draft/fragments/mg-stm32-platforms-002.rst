::area:: STM32 Platforms
::pr:: 120085

::migration-guide::
The STM32MP13 SoC DTSI files have been split per part number so that crypto peripherals (the
``hash`` node) are only described on the variants that provide them. Board device trees must now
include the DTSI matching their exact SoC part number instead of the generic
``stm32mp135.dtsi``. For example, ``stm32mp135f_dk`` now includes
``<st/mp13/stm32mp135f.dtsi>`` instead of ``<st/mp13/stm32mp135.dtsi>``. Out-of-tree boards
based on an STM32MP13 SoC must update their ``#include`` accordingly.
