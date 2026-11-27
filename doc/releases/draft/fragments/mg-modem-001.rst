::area:: Modem
::pr:: 113915

::migration-guide::
Cellular modem chat delimiters and filters are now specified in
:c:struct:`modem_cellular_vendor_config`, not :c:struct:`modem_cellular_data`.

Cellular modem instance PPP pointer is now automatically populated in
:c:struct:`modem_cellular_config`. Assignment to :c:struct:`modem_cellular_data` must be removed.
