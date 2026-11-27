::area:: Drivers: Haptics
::pr:: 105683

::migration-guide::
The ``cirrus,cs40l5x`` compatible has been replaced by variant-specific compatibles
:dtcompatible:`cirrus,cs40l50`, :dtcompatible:`cirrus,cs40l51`, :dtcompatible:`cirrus,cs40l52`,
and :dtcompatible:`cirrus,cs40l53`. Applications using the old compatible must update their
devicetree nodes accordingly.
