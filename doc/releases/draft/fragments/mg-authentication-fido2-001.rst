::area:: Authentication: FIDO2
::pr:: 116552

::migration-guide::
The FIDO2 transport callback API has changed. The
:c:type:`fido2_transport_recv_cb_t` callback now returns an ``int`` to
indicate whether a received message was accepted by the FIDO2 core, and
:c:type:`fido2_transport_cancel_cb_t` now takes the
:c:struct:`fido2_transport` instance that received the cancel command.
Out-of-tree transports must be updated to handle the receive callback
return value and pass the transport instance when invoking the cancel
callback.

Application-provided user-presence backends selected with
:kconfig:option:`CONFIG_FIDO2_UP_CUSTOM` must now implement
:c:func:`fido2_up_reset` to clear their state for each new request.
