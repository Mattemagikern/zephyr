::area:: Networking
::pr:: 118347

::migration-guide::
The HTTP client response callback (:c:type:`http_response_cb_t`) may now be
invoked more than once for a single received buffer, once per body fragment,
for example once per chunk of a chunked response. Applications that assumed a
single callback per receive must append every fragment they are handed.
