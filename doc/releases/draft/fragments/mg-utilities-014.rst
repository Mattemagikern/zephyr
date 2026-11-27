::area:: Utilities
::pr:: 113483

::migration-guide::
The CPU load metric module has been merged into the unified :ref:`cpu_load` module. The
:kconfig:option:`CONFIG_CPU_LOAD_METRIC` option is deprecated; enable
:kconfig:option:`CONFIG_CPU_LOAD` with the :kconfig:option:`CONFIG_CPU_LOAD_BACKEND_RUNTIME_STATS`
backend instead. The ``<zephyr/sys/cpu_load_metric.h>`` header now simply includes
``<zephyr/sys/cpu_load.h>``, and :c:func:`cpu_load_metric_get` is a deprecated wrapper around
:c:func:`cpu_load_get_cpu`. Note that :c:func:`cpu_load_get_cpu` returns the load in per mille
(0...1000) rather than percent; use :c:macro:`CPU_LOAD_PERMILLE_TO_PERCENT` to convert.
