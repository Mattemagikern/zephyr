::area:: Boards/SoCs
::pr:: 118433

::migration-guide::
On the Arduino UNO R4 WiFi, ``zephyr,console`` and ``zephyr,shell-uart`` now
default to SCI9, which the on-board ESP32-S3 bridges to the USB-C connector as
a USB CDC ACM port, instead of SCI2 on the D0/D1 header pins. Console output is
now visible on the same port used to flash the board, with no external
USB-serial adapter. Applications that relied on the console being on D0/D1 can
select it again in an application overlay:

.. code-block:: devicetree

   / {
       chosen {
           zephyr,console = &uart2;
           zephyr,shell-uart = &uart2;
       };
   };

The Arduino UNO R4 Minima is unaffected.
