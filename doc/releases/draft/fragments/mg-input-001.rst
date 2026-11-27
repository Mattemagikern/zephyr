::area:: Input
::pr:: 108294

::migration-guide::
The ``no-disconnect`` property of :dtcompatible:`gpio-keys` has been replaced by enumeration
property ``zephyr,suspend-action``. The new property currently has three values, two of which
are direct replacements for the old situations:

  * ``zephyr,suspend-action = "none";`` is the remplacement for the behavior when
    when ``no-disconnect`` was present. Users of the ``no-disconnect`` property
    should replace it with ``zephyr,suspend-action = "none";``.

  * ``zephyr,suspend-action = "disconnect-with-pupd";`` is the replacement for the
    behavior when ``no-disconnect`` was not present. This is selected by default
    for backwards compatibility with the previous behavior.

  * ``zephyr,suspend-action = "full-disconnect";`` is a new value.
    (Refer to :dtcompatible:`the binding <gpio-keys>` for more details)

Users of the default configuration are advised to reconsider whether it really is appropriate;
migration to ``zephyr,suspend-action = "full-disconnect";`` is recommended.
