::area:: Common Architecture Interface
::pr:: 112103

::migration-guide::
``CONFIG_XTENSA_BACKTRACE_EXCEPTION_DUMP_HOOK`` is removed, since backtrace is now always
using :c:macro:`EXCEPTION_DUMP` for output.
