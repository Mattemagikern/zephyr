::area:: Utilities
::pr:: 117031

::migration-guide::
The internal ``__ASSERT_ON`` define has been removed. Out-of-tree code should invoke
``__ASSERT()`` or ``__ASSERT_NO_MSG()`` directly, as these macros already compile out when
assertions are disabled.
Mark values used only by assertions with ``__maybe_unused`` or ``ARG_UNUSED()`` as appropriate.
