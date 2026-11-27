::area:: MCU Manager
::pr:: 118346

::migration-guide::
:c:func:`smp_client_single_response` takes the SMP transport the response was received on as
a new first argument, and ``res_hdr`` must be in host byte order. A response now only
completes a pending command that was sent on that transport and has the same group and
command ID. Responses from a server that does not echo the group and command ID are ignored,
and the command is retried until it times out.
