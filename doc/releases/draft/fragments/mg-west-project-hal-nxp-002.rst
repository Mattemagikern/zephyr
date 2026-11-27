::area:: West project: hal_nxp
::pr:: 107117

::migration-guide::
S32K344: The pinmux header file for this SoC was renamed from ``S32K344-172MQFP-pinctrl.h`` to
``S32K344_K324_K314_172HDQFP-pinctrl.h``. Out-of-tree boards must update their include directive accordingly::

  #include <nxp/s32/S32K344_K324_K314_172HDQFP-pinctrl.h>
