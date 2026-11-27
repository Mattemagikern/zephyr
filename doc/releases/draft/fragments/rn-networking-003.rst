::area:: Networking
::pr:: 119911

::release-notes::
CoAP

* The CoAP server accepts an observe registration that carries an empty token, which
  :rfc:`7641` allows, and keys the observer on the endpoint and that empty token.
