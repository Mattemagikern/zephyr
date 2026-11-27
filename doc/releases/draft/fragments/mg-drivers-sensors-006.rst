::area:: Drivers: Sensors
::pr:: 116012

::migration-guide::
:dtcompatible:`ti,tmag5273` now interprets ``SENSOR_ATTR_FULL_SCALE`` in Gauss, matching the
unit of the ``SENSOR_CHAN_MAGN_*`` channels, instead of millitesla. Applications using
``TMAG5273_DT_AXIS_RANGE_RUNTIME`` must multiply the values they pass to and read back from
:c:func:`sensor_attr_set` and :c:func:`sensor_attr_get` by 10, e.g. 80 (mT) becomes 800 (G).
