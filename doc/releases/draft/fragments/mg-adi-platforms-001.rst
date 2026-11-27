::area:: ADI Platforms
::pr:: 117081

::migration-guide::
:kconfig:option:`CONFIG_NUM_IRQS` is now computed automatically for all MAX32 SoCs from the
devicetree, based on active (``status = "okay";``) devices, using the
``dt_highest_controller_irq_number`` Kconfig preprocessor function. The hardcoded per-SoC values
have been removed, and the resulting IRQ table is typically considerably smaller than before.
Applications which register custom ISRs (using :c:macro:`IRQ_CONNECT()`) may encounter build
failures such as the following due to :kconfig:option:`CONFIG_NUM_IRQS` having a lower value:

.. code-block::

  gen_isr_tables.py: error: IRQ 88 (offset=0) exceeds the maximum of 54

Explicitly set :kconfig:option:`CONFIG_NUM_IRQS` to an appropriate value to solve these issues.
(:ref:`The following documentation page <setting_configuration_values>` explains how to do it)

Applications that install ISRs at runtime with :c:func:`irq_connect_dynamic` are not covered by
this build-time check and must be reviewed manually.
