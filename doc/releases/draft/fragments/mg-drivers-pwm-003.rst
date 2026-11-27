::area:: Drivers: PWM
::pr:: 112088

::migration-guide::
:dtcompatible:`nxp,ctimer-pwm` now routes its input capture signal through the generic
:ref:`mux <mux_api>` subsystem. The ``inputmux-connections`` property has been removed; describe
the routing with an INPUTMUX controller node (:dtcompatible:`nxp,inputmux`) and reference it from
the timer node's ``mux-states`` property instead.

:dtcompatible:`nxp,sctimer-pwm` now routes its input capture signal through the generic
:ref:`mux <mux_api>` subsystem. The ``input-channels`` property has been removed; describe the
routing with an INPUTMUX controller node (:dtcompatible:`nxp,inputmux`) and reference it from the
timer node's ``mux-states`` property instead.
