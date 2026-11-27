::area:: Networking
::pr:: 117282

::migration-guide::
``CONFIG_NET_TEST_PROTOCOL``, a JSON control channel that let an out of tree
TTCN-3 suite drive the TCP stack and read its internal state, has been
removed, along with the ``samples/net/sockets/tcp`` sample that was its only
system under test. Nothing in the tree enabled the option, and the code
behind it had not compiled for several years. The suites that used it were
archived by their author.

Enabling it also turned off initial sequence number randomisation and made
``net_tcp_connect()`` return without waiting for the connection, so a build
that had it did not behave like one that did not.

There is no replacement option, because the replacement is not an option: the
conformance tests under :zephyr_file:`tests/net/conformance` drive an
unaltered build over the network instead, including a TCP suite covering the
same ground. See :ref:`ttcn3_testing`.
