::area:: Boards/SoCs
::pr:: 108041

::migration-guide::
The Nordic sysbuild Kconfig option ``SB_CONFIG_NRF_HALTIUM_GENERATE_UICR``
has been renamed to :kconfig:option:`SB_CONFIG_NRF_GENERATE_UICR`.
Update sysbuild configurations to use the new name.

The Nordic SoC headers :file:`<haltium_power.h>` and :file:`<haltium_pm_s2ram.h>`
have been renamed to :file:`<soc_power.h>` and :file:`<soc_pm_s2ram.h>` respectively.
Forwarder headers under the old names remain available and emit a ``#warning``
pointing to the new include paths. Out-of-tree code that includes the old paths
should be updated:

* ``#include <haltium_power.h>`` with ``#include <soc_power.h>``.
* ``#include <haltium_pm_s2ram.h>`` with ``#include <soc_pm_s2ram.h>``.
