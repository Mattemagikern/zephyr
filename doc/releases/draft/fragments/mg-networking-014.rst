::area:: Networking
::pr:: 112534

::migration-guide::
LLMNR support has been deprecated. The Kconfig options
:kconfig:option:`CONFIG_LLMNR_RESOLVER` and
:kconfig:option:`CONFIG_LLMNR_RESPONDER` will be removed in a future release
(4.7 at the earliest). LLMNR (RFC 4795) is being retired by Microsoft and is
disabled by default on modern Windows. Applications that rely on local name
resolution should migrate to mDNS (:kconfig:option:`CONFIG_MDNS_RESOLVER` /
:kconfig:option:`CONFIG_MDNS_RESPONDER`).
