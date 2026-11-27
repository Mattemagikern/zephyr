::area:: Logging
::pr:: 116749

::migration-guide::
The UART dictionary log parsing script ``scripts/logging/dictionary/log_parser_uart.py`` has
been removed. Use :zephyr_file:`scripts/logging/dictionary/live_log_parser.py` instead, which
takes the port and baud rate after a ``serial`` sub-command.
