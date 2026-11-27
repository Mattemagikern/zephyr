::area:: Drivers: ESPI
::pr:: 119315

::migration-guide::
The board power node compatible of the :zephyr:code-sample:`espi` sample has been renamed from
``intel-rvp,board-power`` to ``intel,rvp-board-power``. Out-of-tree board overlays for this
sample must use the new compatible.
