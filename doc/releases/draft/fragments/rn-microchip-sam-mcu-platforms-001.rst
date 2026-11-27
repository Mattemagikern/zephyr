::area:: Microchip SAM MCU Platforms
::pr:: 123245

::release-notes::
The default ``west flash`` runner for the Microchip development boards with PIC32C family
devices has changed from ``jlink`` to ``mplab_ipe`` (the MPLAB IPE programmer, invoking
``ipecmd``). Pass ``--runner jlink`` to keep using J-Link.
