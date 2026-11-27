::area:: Common Architecture Interface
::pr:: 116747

::migration-guide::
``CONFIG_SSE`` and ``CONFIG_SSE_FP_MATH`` have been removed. Use
:kconfig:option:`CONFIG_X86_SSE` and :kconfig:option:`CONFIG_X86_SSE_FP_MATH`
instead.

``CONFIG_PLATFORM_SPECIFIC_INIT`` and its ``z_arm_platform_init()`` hook have
been removed. Enable :kconfig:option:`CONFIG_SOC_RESET_HOOK` and rename the
hook to :c:func:`soc_reset_hook`. The new hook runs later in the reset path,
after the stack pointers have been set up, and is skipped on resume from
suspend-to-RAM.
