::area:: RTIO
::pr:: 122530

::migration-guide::
A canceled submission now produces a completion queue event, where cancellation used to be
silent. A thread waiting in :c:func:`rtio_submit` for a submission that is then canceled is
released instead of waiting forever. :c:macro:`RTIO_SQE_NO_RESPONSE` is the only thing that
suppresses the completion, for a canceled submission as for one that runs.

Cancellation is best effort. A submission that is running or about to run may beat the
cancellation and complete with the result its iodev produced. One that is stopped completes
with ``-ECANCELED``, and cascades along a chain the way a failure does, so each submission
behind it also reports ``-ECANCELED``. Consumers that drain a fixed number of completions, or
that treat any completion as data, need to expect the extra events and read ``cqe->result``.
