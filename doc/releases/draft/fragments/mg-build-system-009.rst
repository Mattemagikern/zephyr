::area:: Build system
::pr:: 117326

::migration-guide::
The ``ZephyrUnittest`` CMake package, deprecated since Zephyr 3.1, has been removed and
``west zephyr-export`` no longer registers it. Use
``find_package(Zephyr COMPONENTS unittest)`` instead of ``find_package(ZephyrUnittest)``.
