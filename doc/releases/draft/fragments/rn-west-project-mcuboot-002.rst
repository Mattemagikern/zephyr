::area:: West project: mcuboot
::pr:: 118393

::release-notes::
Espressif boards no longer force overwrite-only mode and unsigned images under sysbuild.
They now build a swap-using-offset MCUboot with rollback and an RSA-2048 signed
application, and the shared Espressif partition tables no longer reserve a scratch
partition. See the :ref:`migration guide <migration_4.5>`.
