::area:: Drivers: GPIO
::pr:: 104690

::migration-guide::
The STM32 GPIO driver now returns ``-EINVAL`` when attempting to configure a GPIO pin in disabled
state with a pull-up/pull-down resistor using :c:func:`gpio_pin_configure`. The driver would
previously return ``0`` without actually honoring those flags (no PU/PD resistor was enabled).
Applications encountering this error should remove :c:macro:`GPIO_PULL_UP`/ :c:macro:`GPIO_PULL_DOWN`
from the ``flags`` they provide to :c:func:`gpio_pin_configure`; this will result in the same
behavior as before since these flags were effectively ignored.

On STM32F1 series, GPIO output pins now use 50 MHz max. speed instead of 10 MHz.
