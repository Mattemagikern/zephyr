::area:: Drivers: Fuel Gauge
::pr:: 112276

::migration-guide::
Drivers had inconsistently been reporting full charge/discharge cycles or
"1/100ths" of a cycle in the ``FUEL_GAUGE_CYCLE_COUNT`` property.
The property now consistently reports full cycles, and drivers that
previously reported fractions of a cycle (i.e. ADP5360 and BQ27Z746) have
been updated to report full cycles instead.
Applications that relied on the old behavior should be updated.
