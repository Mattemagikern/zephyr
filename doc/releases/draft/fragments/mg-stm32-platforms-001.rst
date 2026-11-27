::area:: STM32 Platforms
::pr:: 104690, 108294

::migration-guide::
:dtcompatible:`gpio-keys` devices will fail to suspend unless property ``zephyr,suspend-action``
is present in Devicetree with value ``"none"`` or ``"full-disconnect"``. Refer to the migration
guide entry related to this binding for more details. (:github:`104690` / :github:`108294`)
