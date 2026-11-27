::area:: USB
::pr:: 108118

::migration-guide::
Indicating protocol error via ``errno`` in control transfer handlers is deprecated.
Handlers should return error code directly.
