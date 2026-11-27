::area:: Common Architecture Interface
::pr:: 113218

::migration-guide::
SoCs using :kconfig:option:`CONFIG_XTENSA_BACKTRACE` are now expected to implement
:c:func:`xtensa_soc_stack_ptr_is_sane` and :c:func:`xtensa_soc_ptr_executable`.
