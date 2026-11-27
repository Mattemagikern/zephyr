::area:: Utilities
::pr:: 112756

::migration-guide::
Several legacy assertion Kconfig options are deprecated in favor of the per-module ZASSERT
levels (``CONFIG_ASSERT_MODULE_<module>_LEVEL``, defaulting through the ``DEFAULT`` module):

* :kconfig:option:`CONFIG_ASSERT_VERBOSE` maps to
  :kconfig:option:`CONFIG_ASSERT_MODULE_DEFAULT_LEVEL_VERBOSE`.
* :kconfig:option:`CONFIG_ASSERT_NO_COND_INFO`, :kconfig:option:`CONFIG_ASSERT_NO_MSG_INFO` and
  :kconfig:option:`CONFIG_ASSERT_NO_FILE_INFO` map to
  :kconfig:option:`CONFIG_ASSERT_MODULE_DEFAULT_LEVEL_TERSE`.
* ``CONFIG_FORCE_NO_ASSERT`` is replaced by setting :kconfig:option:`CONFIG_ASSERT` to ``n``.
* ``CONFIG_ASSERT_LEVEL`` is replaced by the ``DEFAULT`` module level; a level of ``0`` maps to
  :kconfig:option:`CONFIG_ASSERT_MODULE_DEFAULT_LEVEL_OFF`.

The assertion hooks ``assert_post_action`` and ``assert_print`` have been removed.
Use the new ``zassert_post_action`` and ``zassert_vprint`` hooks to achieve equivalent functionality.
Note: The ``zassert_post_action`` hook is a terminal function, returning to the caller is not supported.
If the application needs to return to the caller, enabling :kconfig:option:`CONFIG_ASSERT_TEST` will
declare the ``zassert_post_action`` hook as a non-terminal function, allowing
execution to continue after the hook runs.

:kconfig:option:`CONFIG_ASSERT_NO_FILE_INFO` does not change the ``zassert_post_action`` hook
signature like the old ``assert_post_action`` hook did.

The zassert hook ``zassert_vprint``, the replacement for ``assert_print``, is now called with a
``va_list`` argument instead of a variable number of arguments. Out-of-tree code that implements
this hook must be updated.
