.. zephyr:code-sample:: workq-fifo
   :name: FIFO Work Queue Sample

   Offload work to a FIFO work queue serviced by several worker threads.

Overview
********

A sample demonstrating the FIFO variant of the Zephyr work queue. Ready items are
executed in submission order by two worker threads bound to the same queue, so
items are dequeued in order but may complete out of order.

The sample runs two scenarios:

* ``sample_multiple_workers()`` submits a batch of delayed items followed by a
  batch of immediate ones, then waits for the queue to drain.
* ``sample_synchronization()`` shows the recommended way to block until a
  specific item has finished, by embedding a semaphore in the work item.

Building and Running
********************

This application can be built and executed on QEMU as follows:

.. zephyr-app-commands::
   :zephyr-app: samples/kernel/workqs/fifo
   :host-os: unix
   :board: qemu_x86
   :goals: run
   :compact:

To build for another board target, replace "qemu_x86" above with it.

Sample Output
=============

.. code-block:: console


Exit QEMU by pressing :kbd:`CTRL+A` :kbd:`x`.
