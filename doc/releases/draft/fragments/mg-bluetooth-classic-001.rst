::area:: Bluetooth Classic
::pr:: 108022

::migration-guide::
The BR/EDR specific callbacks ``role_changed`` and ``br_mode_changed`` in
:c:struct:`bt_conn_cb` have been moved into a new sub-struct
:c:struct:`bt_conn_br_cb`, accessible via the ``br`` member. Application code
using these callbacks must update the designated initializers:

* ``.role_changed`` → ``.br.role_changed``
* ``.br_mode_changed`` → ``.br.mode_changed``
