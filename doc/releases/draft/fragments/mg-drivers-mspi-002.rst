::area:: Drivers: MSPI
::pr:: 108082

::migration-guide::
MSPI device binding filenames now use ``(vendor,)device-mspi.yaml`` for
MSPI-specific variants, while the devicetree ``compatible`` strings describe
the device itself instead of encoding the MSPI bus. Boards, shields, samples,
tests, and out-of-tree devicetree overlays are recommended to update MSPI
child node compatibles as follows:

* ``jedec,mspi-nor`` -> ``jedec,nor``
* ``mspi-atxp032`` -> ``atxp032``
* ``mspi-is25xX0xx`` -> ``is25xX0xx``
* ``mspi-aps6404l`` -> ``aps6404l``
* ``mspi-aps-z8`` -> ``aps-z8``
* ``zephyr,mspi-emul-device`` -> ``zephyr,emul-device-mspi``
* ``zephyr,mspi-emul-flash`` -> ``zephyr,emul-flash``

It is recommended that out-of-tree MSPI device drivers should likewise update
``DT_DRV_COMPAT`` and generated devicetree Kconfig symbol references to the new compatible names.
If a driver, sample, or test must ensure that one of these generic
compatibles is instantiated on an MSPI bus, add an explicit MSPI bus check,
such as ``dt_compat_on_bus`` in Kconfig or ``dt_compat_on_bus`` filters in
test metadata.
