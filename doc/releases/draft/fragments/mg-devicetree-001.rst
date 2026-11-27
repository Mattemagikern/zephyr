::area:: Devicetree
::pr:: 107271

::migration-guide::
``int`` and ``array`` typed devicetree properties whose DTS source uses a negative literal
(e.g. ``<(-1)>``) now expand to a negative value instead of the two's-complement unsigned
value used previously. Code that relied on the old unsigned representation, for example
unsigned comparisons or ``BUILD_ASSERT(DT_PROP(node, foo) > 0, ...)`` checks, must be updated
to use signed types or signed-aware checks.
