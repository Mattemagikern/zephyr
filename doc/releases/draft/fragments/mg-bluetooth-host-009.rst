::area:: Bluetooth Host
::pr:: 110687

::migration-guide::
:c:func:`bt_addr_le_to_str` now formats LE addresses with a single-character type prefix,
``P:`` for public and ``R:`` for random, directly followed by the address, e.g.
``R:11:22:33:44:55:66``. The previous ``11:22:33:44:55:66 (random)`` form is no longer
produced, and address types carrying additional HCI-level bits, such as
``BT_ADDR_LE_RANDOM_ID``, are formatted by their base type rather than as ``(random-id)`` or
a raw hex value. Code that parses Zephyr log or shell output to extract addresses must be
updated. :c:macro:`BT_ADDR_LE_STR_LEN` has shrunk from ``30`` to ``20`` accordingly.

:c:func:`bt_addr_le_from_str` no longer takes a separate address type string. It accepts only
the ``P:``/``R:`` prefixed format produced by :c:func:`bt_addr_le_to_str`; the previous
``"XX:XX:XX:XX:XX:XX"`` + ``"public"``/``"random"`` form is not supported. All Bluetooth
shell commands that take an LE address (for example ``bt connect``, ``bt disconnect``,
``bt clear``, ``bt fal-add``, ``bt per-adv-sync-create``, ``gatt resubscribe`` and
``bap_broadcast_assistant add_src``) consequently take it as a single
``P:XX:XX:XX:XX:XX:XX`` or ``R:XX:XX:XX:XX:XX:XX`` argument instead of an address followed
by a separate type argument.
