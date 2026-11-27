::area:: Networking
::pr:: 120711

::release-notes::
MLD

* Nodes now answer Multicast Address Specific Queries, delay query responses by a random
  time within the Maximum Response Delay, retransmit the unsolicited report of a join,
  switch to MLDv1 when an MLDv1 querier is present and drop queries without a link-local
  source or the Router Alert option, as required by :rfc:`2710` and :rfc:`3810`. No report
  is sent for the all-nodes group any more, and all groups are reported again once the
  link-local address of the interface is valid. The number of report transmissions follows
  the new :kconfig:option:`CONFIG_NET_IPV6_MLD_ROBUSTNESS`, and disabling the new
  :kconfig:option:`CONFIG_NET_IPV6_MLD_V1_COMPAT` makes the node ignore MLDv1 messages.
