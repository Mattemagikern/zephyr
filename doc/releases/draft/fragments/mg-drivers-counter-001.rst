::area:: Drivers: Counter
::pr:: 112088

::migration-guide::
:dtcompatible:`nxp,lpc-ctimer` now routes its input capture signal through the generic
:ref:`mux <mux_api>` subsystem. The ``inputmux-connections`` property has been removed; describe
the routing with an INPUTMUX controller node (:dtcompatible:`nxp,inputmux`) and reference it from
the timer node's ``mux-states`` property instead. The cell layout is unchanged, so an existing
``inputmux-connections = <&inputmux0 0 0x06000024>;`` becomes
``mux-states = <&inputmux0 0 0x06000024>;``
