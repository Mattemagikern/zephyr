::area:: Drivers: Mbox
::pr:: 116720

::migration-guide::
The :dtcompatible:`renesas,rz-mhu-mbox` driver was reworked so that a single MHU unit
serves both TX and RX on one MBOX channel, instead of dedicating each channel to one
direction. A single driver instance can now own several channels. Devicetree nodes using
this compatible must be updated:

* ``channel`` was renamed to ``unit``, because it indexes the underlying MHU hardware unit
  and not the MBOX channel. The two numbering schemes are independent.
* ``tx-mask`` and ``rx-mask`` were replaced by a single ``channel-mask``, a bitmask of the
  valid MBOX channels where bit ``n`` enables channel ``n``.
* ``channels-count`` must match the number of ``interrupt-names`` entries on the node.
  This is now enforced at build time.
* ``shared-memory`` is now optional and should be left unset in most cases. The shared
  memory is instead provided by a single ``zephyr,memory-region`` node named ``mhu_shmem``,
  which replaces the per-unit ``mmio-sram`` nodes and makes the linker emit
  ``__mhu_shmem_start`` for the FSP MHU driver. Boards without this memory region fail to
  link with an undefined reference to that symbol.

For example:

.. code-block:: devicetree

   /* Before */
   mhu3_shm: memory@62f01018 {
           compatible = "mmio-sram";
           reg = <0x62f01018 0x8>;
   };

   mbox3: mhu@40400060 {
           channel = <3>;
           tx-mask = <0x00000002>;
           rx-mask = <0x00000001>;
           shared-memory = <&mhu3_shm>;
   };

   /* After */
   mhu_shmem: memory-region@62f01000 {
           compatible = "zephyr,memory-region";
           reg = <0x62f01000 0x1000>;
           zephyr,memory-region = "mhu_shmem";
   };

   mbox3: mhu@40400060 {
           unit = <3>;
           channel-mask = <0x1>;
   };
