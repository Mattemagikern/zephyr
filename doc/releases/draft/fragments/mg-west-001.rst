::area:: West
::pr:: 116749

::migration-guide::
The ``--skip-rebuild`` option of the ``west`` commands that invoke a runner (``flash``,
``debug``, ``debugserver``, ``attach``, ``rtt``, ``reset``, ``robot`` and ``simulate``) has
been removed. Use ``--no-rebuild`` instead.

The ``PYOCD_DAPARG`` environment variable is no longer read by the ``pyocd`` runner. Pass
``--daparg`` to ``west flash``/``west debug`` instead.
