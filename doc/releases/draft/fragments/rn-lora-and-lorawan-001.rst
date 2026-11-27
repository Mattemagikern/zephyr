::area:: LoRa and LoRaWAN
::pr:: 105077

::release-notes::
Added a native LoRaWAN backend
(:kconfig:option:`CONFIG_LORA_MODULE_BACKEND_NATIVE`) that implements
LoRaWAN 1.0.x Class A directly on top of the LoRa radio driver, without
the Semtech LoRaMac-node dependency.  Currently supports the EU868 and
KR920 regions, the latter including the listen-before-talk that band
uses in place of a duty cycle.
