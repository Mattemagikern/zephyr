::area:: Networking
::pr:: 116050

::migration-guide::
The ``struct dns_server`` type nested in :c:struct:`dns_resolve_context` has been
renamed to ``struct dns_server_info``. A C++ class member cannot share the name of
its enclosing class, so the old tag made ``<zephyr/net/dns_resolve.h>`` impossible to
include from C++. No field was renamed, so accesses such as
``ctx->servers[i].dns_server_addr`` are unaffected; only code that names the type
itself, for example in a ``CONTAINER_OF()`` call, needs updating.
