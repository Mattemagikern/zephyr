::area:: Networking: Wi-Fi
::pr:: 115242

::release-notes::
The transmit power ceiling properties in ``wifi-tx-power-2g.yaml`` and
``wifi-tx-power-5g.yaml`` are no longer ``required`` and now carry
conservative defaults, so a board that has not been characterised errs on
the side of transmitting too little rather than exceeding a regulatory
limit. Boards that have measured their own limits continue to state them
explicitly, so no board changes behaviour.
