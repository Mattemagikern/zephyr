.. _workqs:

Work Queues
###########

A work queue defers processing from an interrupt handler or a high-priority
thread to one or more dedicated worker threads. Zephyr provides several work
queue implementations; each page below documents one of them in full.

.. toctree::
   :maxdepth: 1

   k_work.rst
   fifo.rst
   priority.rst
