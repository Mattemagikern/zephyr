::area:: Shell
::pr:: 116751

::migration-guide::
The ``kernel log_level <module> <severity>`` shell command, deprecated since Zephyr v4.1.0,
has been removed. Use ``log enable <severity> <module>`` instead: the arguments are reversed
and the severity is a name (``none``, ``err``, ``wrn``, ``inf``, ``dbg``), not a number.
