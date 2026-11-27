::area:: Drivers: CAN
::pr:: 117889

::migration-guide::
The CAN controllers driver ops no longer contain a ``can_set_state_change_callback_t`` function
pointer as adding/removing callbacks is now handled via the generic
:c:func:`can_add_state_change_callback`, and :c:func:`can_remove_state_change_callback` API
functions. Out-of-tree drivers can either remove the driver op completely or replace it with
``can_state_change_callbacks_enabled_t`` as needed. Drivers must now use
:c:func:`can_fire_state_change_callbacks` for firing CAN controller state change callbacks.
