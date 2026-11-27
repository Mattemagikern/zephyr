::area:: Drivers: System timer
::pr:: 117274

::migration-guide::
When :kconfig:option:`SYSTEM_TIMER_LPM_COMPANION_COUNTER` is enabled, the Low-Power
Companion counter, selected by the :ref:`generic chosen <devicetree-zephyr-chosen-nodes>`
``zephyr,system-timer-companion``, is checked at build time and must be usable as
wake-up source. If not already present, the ``wakeup-source`` property should be
added to such nodes to indicate they can be used as wake-up source.

.. note::

  This behavior was already expected in previous Zephyr releases but never asserted.
