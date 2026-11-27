::area:: Networking
::pr:: 109224

::migration-guide::
IPv4 and IPv6 unicast route-table support is now exposed through the
:kconfig:option:`CONFIG_NET_IPV4_ROUTE` and
:kconfig:option:`CONFIG_NET_IPV6_ROUTE` options.

These options control the per-family unicast route tables that are used by
static route management, networking shell route commands, and host-side route
selection for locally originated traffic such as VPN-bound packets. They do
not, by themselves, enable packet forwarding between interfaces.

The Kconfig options :kconfig:option:`CONFIG_NET_IPV4_ROUTING` and
:kconfig:option:`CONFIG_NET_IPV6_ROUTING` have been renamed to
:kconfig:option:`CONFIG_NET_IPV4_FORWARDING` and
:kconfig:option:`CONFIG_NET_IPV6_FORWARDING`.

The renamed options explicitly describe IP forwarding between interfaces.
Applications that only need route-table lookups or static routes should
enable :kconfig:option:`CONFIG_NET_IPV4_ROUTE` or
:kconfig:option:`CONFIG_NET_IPV6_ROUTE` and leave forwarding disabled.
Applications acting as routers should enable both the route-table option and
the corresponding forwarding option.

Out-of-tree IPv6 configurations should also migrate away from the deprecated
legacy aliases :kconfig:option:`CONFIG_NET_ROUTE`,
:kconfig:option:`CONFIG_NET_ROUTING`,
:kconfig:option:`CONFIG_NET_MAX_ROUTES`, and
:kconfig:option:`CONFIG_NET_MAX_NEXTHOPS` and use the
:kconfig:option:`CONFIG_NET_IPV6_*` symbols directly.
