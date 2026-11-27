::area:: Common Architecture Interface
::pr:: 107541

::migration-guide::
A new architecture primitive, ``arch_cpu_irqs_are_enabled()``, has been added.
It returns the current interrupt-enable state of the calling CPU without
modifying it, complementing ``arch_irq_unlocked()`` which inspects a saved
key.  Out-of-tree architecture ports must provide an implementation.
