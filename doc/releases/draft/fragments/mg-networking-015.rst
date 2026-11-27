::area:: Networking
::pr:: 112800

::migration-guide::
The following MQTT-SN transport functions now take an :c:struct:`mqtt_sn_transport`
instead of an :c:struct:`mqtt_sn_client`:

* :c:member:`mqtt_sn_transport.recvfrom`
* :c:member:`mqtt_sn_transport.poll`
* :c:member:`mqtt_sn_transport.sendto`
