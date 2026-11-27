::area:: Build system
::pr:: 116673

::migration-guide::
The hardening tool's data file :file:`scripts/kconfig/hardened.csv` has been replaced by a
YAML database: profiles in :file:`scripts/kconfig/hardening.yaml` and per-subsystem
``hardening.yaml`` fragments next to the Kconfig files they relate to. Downstream forks that
patched the CSV should migrate their entries to the new format (see :ref:`hardening`); out-of-tree
recommendations no longer require patching the in-tree file at all and can instead be provided
via ``-DHARDENCONFIG_EXTRA_SOURCES=``. Note that ``CONFIG_DEBUG_COREDUMP`` was listed in the
CSV with a syntax error and was silently skipped; it is now actually checked, so
``hardenconfig`` may report it as a new finding. Three CSV entries were dropped rather than
migrated: ``CONFIG_STACK_USAGE``, which only produces build-time :file:`.su` files and does not
affect the image, and ``CONFIG_MPU_STACK_GUARD`` and ``CONFIG_BUILTIN_STACK_GUARD``, which are
mutually exclusive mechanisms arbitrated by :kconfig:option:`CONFIG_HW_STACK_PROTECTION` —
recommending both flagged the wrong one on cores with stack pointer limit registers. Enabling
:kconfig:option:`CONFIG_HW_STACK_PROTECTION`, which is still recommended, lets the architecture
pick.
