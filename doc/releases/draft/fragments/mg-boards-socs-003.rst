::area:: Boards/SoCs
::pr:: 123426

::migration-guide::
On RP2040, the ``mbox`` node was split into ``mbox0`` and ``mbox1`` to provide
per-core mailbox instances. The ``ipm_mbox`` node was renamed to ``ipm_mbox0`` and
``ipm_mbox1``, respectively.
