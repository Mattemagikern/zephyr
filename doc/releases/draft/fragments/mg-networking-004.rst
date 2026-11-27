::area:: Networking
::pr:: 108520

::migration-guide::
Various IP routing related Kconfig options will have now ``IPV6`` prefix added to
them. This is done so that we can have IPv4 routing symbols that provide same
functionality as IPv6 ones but can be controlled separately.
