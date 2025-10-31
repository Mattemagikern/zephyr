.. zephyr:code-sample:: workq-priority
   :name: Priority Work Queue Sample

   Offload work to a priority work queue and observe priority-ordered execution.

Overview
********

A sample demonstrating the priority variant of the Zephyr work queue. Work items
carry a priority (a lower value is a higher priority); ready items are executed in
ascending priority order, FIFO among equal priorities.

The sample runs two scenarios against a single worker thread, so the execution
order is fully determined by the item priorities:

* ``sample_priority_ordering()`` queues a batch of mixed-priority items *before*
  starting the worker, so the run order reflects priority rather than submission
  timing.
* ``sample_delayed_priority()`` submits items with an identical delay but
  different priorities; they are promoted together and priority then decides
  which runs first.

Building and Running
********************

This application can be built and executed on QEMU as follows:

.. zephyr-app-commands::
   :zephyr-app: samples/kernel/workqs/priority
   :host-os: unix
   :board: qemu_x86
   :goals: run
   :compact:

To build for another board target, replace "qemu_x86" above with it.

Sample Output
=============

.. code-block:: console


Exit QEMU by pressing :kbd:`CTRL+A` :kbd:`x`.
