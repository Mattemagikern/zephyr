::area:: Build system
::pr:: 118297

::migration-guide::
:kconfig:option:`CONFIG_DEPRECATION_TEST` has been deprecated, this is because
:kconfig:option:`CONFIG_WARN_DEPRECATED` can be used instead, simply replace lines with
``CONFIG_DEPRECATION_TEST=y`` with ``CONFIG_WARN_DEPRECATED=n``.
