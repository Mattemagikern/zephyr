::area:: Devicetree
::pr:: 118600

::release-notes::
The I3C shell now enumerates I3C controllers through the ``i3c`` device
class instead of a hardcoded list of compatibles, so it also covers
out-of-tree I3C drivers.
