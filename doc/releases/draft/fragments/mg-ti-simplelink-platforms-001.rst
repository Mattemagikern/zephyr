::area:: TI SimpleLink Platforms
::pr:: 115068

::migration-guide::
Using a :dtcompatible:`ti,am654-timer` instance as system timer now requires
the explicit selection via the :ref:`generic chosen
<devicetree-zephyr-chosen-nodes>` ``zephyr,system-timer`` node, which is
provided for all SoCs that are using it. Downstream boards and applications
that want to use another instance than the SoC default one need to overwrite
this.
