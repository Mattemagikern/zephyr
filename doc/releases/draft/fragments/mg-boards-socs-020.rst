::area:: Boards/SoCs
::pr:: 112474

::migration-guide::
On the nRF54LM20 DK, the ``nrf7002eb2`` shield (and its ``nrf7002eb2_nrf7001`` and
``nrf7002eb2_nrf7000`` variants) no longer redirects the application console. Previously it
disabled UART20 and rerouted the console (shell, mcumgr, and Bluetooth monitor) to UART30 to
work around a pin conflict that only exists on the pre-production kit, not on the production
nRF54LM20 DK. The console now stays on UART20 (VCOM1), matching the standard board behaviour,
and ``button3``/``sw3`` is no longer deleted. Existing users of this shield on the nRF54LM20 DK
must move their serial terminal from VCOM0 back to VCOM1. The nRF54L15 DK is unaffected: its
expansion header genuinely conflicts with UART20, so it keeps rerouting the console to UART30.
