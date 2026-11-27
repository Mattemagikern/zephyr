::area:: West project: trusted-firmware-a
::pr:: 108103

::release-notes::
``CONFIG_TFA_BUILD_FIP`` is introduced to configure FIP (Firmware Image Package) generation.
FIP generation is by default disabled, but can be enabled by setting ``CONFIG_TFA_BUILD_FIP=y``
in ``prj.conf`` or for custom boards, in the board's ``<board>_defconfig`` file.
