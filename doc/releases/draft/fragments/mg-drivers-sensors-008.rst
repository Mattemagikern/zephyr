::area:: Drivers: Sensors
::pr:: 116079

::migration-guide::
The ``pgain``, ``again``, ``ppulse-length`` and ``pled-boost`` properties of
:dtcompatible:`avago,apds9960` used to spell the 2-bit register fields they select in hex
(``0x00``/``0x01``/``0x10``/``0x11``) and now take the physical value they select instead: gain
multipliers for ``pgain`` (``1``/``2``/``4``/``8``) and ``again`` (``1``/``4``/``16``/``64``),
microseconds for ``ppulse-length`` (``4``/``8``/``16``/``32``) and percent for ``pled-boost``
(``100``/``150``/``200``/``300``). Most of the old values are rejected by the new enums, but
``pgain = <0x01>`` and ``again = <0x01>`` still build and now select 1x rather than 2x and 4x,
so update them explicitly. Nodes that do not set these properties are unaffected.
