::area:: USB
::pr:: 108414

::migration-guide::
The USB host controller API struct ``uhc_api`` got renamed to :c:struct:`uhc_driver_api`.
It now also uses :c:macro:`DEVICE_API`. Out-of-tree USB host controller drivers must rename
their API struct definitions and switch their API instances to ``DEVICE_API(uhc, ...)``.
