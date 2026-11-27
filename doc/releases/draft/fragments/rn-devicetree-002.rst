::area:: Devicetree
::pr:: 118594

::release-notes::
Bindings can declare device class membership with the new ``class:`` key
(see :ref:`dt-bindings-class`), enabling build-time enumeration of all
nodes of a device class:

* :c:macro:`DT_NODE_HAS_CLASS`
* :c:macro:`DT_HAS_CLASS_STATUS_OKAY`
* :c:macro:`DT_NUM_CLASS_STATUS_OKAY`
* :c:macro:`DT_FOREACH_CLASS_STATUS_OKAY`
* :c:macro:`DT_FOREACH_CLASS_STATUS_OKAY_VARGS`
* The ``$(dt_class_enabled,<class name>)`` Kconfig preprocessor function

The ADC shell now enumerates ADC controllers through the ``adc`` device
class instead of a hardcoded list of compatibles, so it also covers
out-of-tree ADC drivers.
