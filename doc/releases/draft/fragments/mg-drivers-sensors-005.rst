::area:: Drivers: Sensors
::pr:: 112088

::migration-guide::
:dtcompatible:`nxp,mcux-qdc` now routes its input signals through the generic
:ref:`mux <mux_api>` subsystem. The ``input-channels`` and ``inputmux-connections`` properties
have been removed; describe the routing with a mux controller node (for example
:dtcompatible:`nxp,inputmux`) and reference it from the decoder node's ``mux-states`` property
instead.
