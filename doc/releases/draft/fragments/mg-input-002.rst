::area:: Input
::pr:: 108708

::migration-guide::
Kconfig options for the ft6146, ft5336, and cst8xx input drivers have been renamed to be
consistent with the other input drivers. Applications using the following Kconfig options
must update their configurations accordingly:

* ``CONFIG_INPUT_FT5336_PERIOD`` → :kconfig:option:`CONFIG_INPUT_FT5336_PERIOD_MS`
* ``CONFIG_INPUT_CST8XX_PERIOD`` → :kconfig:option:`CONFIG_INPUT_CST8XX_PERIOD_MS`
* ``CONFIG_INPUT_FT6146_PERIOD`` → :kconfig:option:`CONFIG_INPUT_FT6146_PERIOD_MS`

* Nunchuk driver wronlgy reported ``INPUT_KEY_Z``, respective ``INPUT_KEY_C`` in button events. This
  has been fixed and ``INPUT_BTN_Z``, respective ``INPUT_BTN_C`` is used now.
