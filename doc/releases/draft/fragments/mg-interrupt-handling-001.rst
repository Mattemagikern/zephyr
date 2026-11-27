::area:: Interrupt Handling
::pr:: 107565

::migration-guide::
All interrupt controller bindings now use ``flags`` as the interrupt cell name
instead of ``sense``. The following interrupt controller bindings were updated:

* :dtcompatible:`intel,ioapic`
* :dtcompatible:`intel,loapic`
* :dtcompatible:`cdns,xtensa-core-intc`
* :dtcompatible:`intel,ace-intc`
* :dtcompatible:`intel,cavs-intc`
* :dtcompatible:`snps,designware-intc`
* :dtcompatible:`mediatek,adsp_intc`

Drivers using these interrupt controllers are updated to use ``flags`` as the cell name.
However, any out-of-tree drivers that directly access interrupt properties using
``DT_INST_IRQ(n, sense)`` or ``DT_IRQ(node, sense)`` should be updated to use ``flags`` instead
of ``sense``.
