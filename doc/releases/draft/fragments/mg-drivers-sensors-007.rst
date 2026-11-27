::area:: Drivers: Sensors
::pr:: 112088

::migration-guide::
:dtcompatible:`nxp,mcux-qdec` now routes its input signals through the generic
:ref:`mux <mux_api>` subsystem. The ``xbar`` property has been removed; describe the routing with
a mux controller node (for example :dtcompatible:`nxp,mcux-xbar`) and reference it from the
decoder node's ``mux-states`` property instead.
