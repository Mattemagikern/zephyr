::area:: Networking
::pr:: 120685

::release-notes::
IGMP

* Hosts now answer Group-Specific Queries, delay query responses by a random time within
  the Max Resp Time, retransmit the unsolicited report of a join and switch to IGMPv1 or
  IGMPv2 when a querier of that version is present, as required by :rfc:`2236` and :rfc:`3376`.
  The number of report transmissions follows the new
  :kconfig:option:`CONFIG_NET_IPV4_IGMP_ROBUSTNESS`. Queries without the IP Router Alert
  option are ignored when :kconfig:option:`CONFIG_NET_IPV4_IGMP_REQUIRE_ROUTER_ALERT`
  is enabled.
