::area:: Utilities
::pr:: 117324

::migration-guide::
The ``CONFIG_ZTEST_SHUFFLE_SUITE_REPEAT_COUNT`` and ``CONFIG_ZTEST_SHUFFLE_TEST_REPEAT_COUNT``
Kconfig options, deprecated since Zephyr 4.0, have been removed. With
:kconfig:option:`CONFIG_ZTEST_SHUFFLE` alone, suites and tests now run once per execution, in a
shuffled order; to repeat them, enable :kconfig:option:`CONFIG_ZTEST_REPEAT` and set
:kconfig:option:`CONFIG_ZTEST_SUITE_REPEAT_COUNT` and
:kconfig:option:`CONFIG_ZTEST_TEST_REPEAT_COUNT`.
