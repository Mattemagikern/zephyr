::area:: Display controllers
::pr:: 115633

::migration-guide::
:dtcompatible:`raspberrypi,bcm2711-framebuffer` now requires a ``pixel-format``
property and gains an optional ``red-blue-swap`` boolean to indicate the panel expects
BGR channel order. Boards relying on firmware-negotiated pixel order to correct swapped
channels must also set ``red-blue-swap``.
