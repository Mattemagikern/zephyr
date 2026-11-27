::area:: LoRa and LoRaWAN
::pr:: 108813

::migration-guide::
The native LoRaWAN backend
(:kconfig:option:`CONFIG_LORA_MODULE_BACKEND_NATIVE`) now requires
:c:func:`lorawan_start` before the following runtime configuration APIs are
accepted:

* :c:func:`lorawan_set_datarate` and :c:func:`lorawan_set_conf_msg_tries`
  return ``-EPERM`` if called before start.
* :c:func:`lorawan_enable_adr` (which has a ``void`` return type) logs a
  warning and drops the call if invoked before start.

Configuration values previously latched by these APIs before
:c:func:`lorawan_start` are no longer retained.  Applications that called
them during initialization must move the calls to after start. They may still
run before or after a successful :c:func:`lorawan_join`.

:c:func:`lorawan_set_channels_mask` is unaffected and remains callable any
time after :c:func:`lorawan_start`, since the channel mask influences the
Join-Request channel selection itself.

These ordering requirements do not apply to the LoRaMac-node backend
(:kconfig:option:`CONFIG_LORA_MODULE_BACKEND_LORAMAC_NODE`).
