::area:: Boards/SoCs
::pr:: 113140

::migration-guide::
:zephyr:board:`rak4631` now supports the WisBlock ecosystem. A WisBlock Base
Board shield (e.g. :ref:`rakwireless_rak19007`) is required to expose the
sensor and IO slots:

.. code-block:: shell

   west build -b rak4631/nrf52840 --shield rakwireless_rak19007
