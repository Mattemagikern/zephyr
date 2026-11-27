::area:: Twister
::pr:: 116359

::migration-guide::
Faults after tests have passed are now explicitly detected and fail the whole
testsuite, if a test produces a fault on purpose then the corresponding test
case has to be marked with ``ignore_faults: true``.
