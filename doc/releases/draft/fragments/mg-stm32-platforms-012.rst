::area:: STM32 Platforms
::pr:: 112999

::migration-guide::
Using the ``stm32_lp_tick_source`` nodelabel to select an LPTIM as system timer is no longer supported
and will trigger a build error. Use the :ref:`generic chosen <devicetree-zephyr-chosen-nodes>`
``zephyr,system-timer`` instead.
