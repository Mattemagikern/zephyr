::area:: Boards/SoCs
::pr:: 117118

::migration-guide::
The STMicroelectronics ``stm32mp157c_dk2`` board has been renamed to
``stm32mp157x_dk2`` and now references both SoC variants. Build for the
STM32MP157C-DK2 with ``-b stm32mp157x_dk2/stm32mp157cxx`` and for the
STM32MP157F-DK2 with ``-b stm32mp157x_dk2/stm32mp157fxx``. The previous
``-b stm32mp157c_dk2`` is deprecated and now maps to
``stm32mp157x_dk2/stm32mp157cxx``.
