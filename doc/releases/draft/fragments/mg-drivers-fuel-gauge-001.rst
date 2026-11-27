::area:: Drivers: Fuel Gauge
::pr:: 104236

::migration-guide::
Various fuel gauge property enums and union fields have been deprecated in
favor of new versions with explicit unit suffixes. Applications and drivers
should migrate to the unit-suffixed names. For example,
``FUEL_GAUGE_CURRENT`` (``val.current``) is replaced by
``FUEL_GAUGE_CURRENT_UA`` (``val.current_ua``).
