::area:: Common Architecture Interface
::pr:: 112456

::migration-guide::
The ARMv7-M MPU device-type region attributes ``REGION_PPB_ATTR``, ``REGION_IO_ATTR``
and ``REGION_EXTMEM_ATTR`` now set Execute-Never (``XN=1``) on all ARMv7-M cores.
Executing from Device/Strongly-ordered memory is architecturally UNPREDICTABLE on
ARMv7-M, so no valid use case is affected. On Cortex-M7 the XN attribute additionally
prevents speculative instruction fetches into these regions, which can cause bus hangs
or read side effects in peripheral space (Arm Cortex-M7 TRM, "Speculative accesses -
Considerations for system design"); the memory type alone does not prevent them.
Out-of-tree boards that nevertheless execute from a region mapped with these
attributes must define a custom attribute instead.

The new :kconfig:option:`CONFIG_ARM_MPU_CM7_UNMAPPED_REGION` option makes the Arm
MPU driver program the lowest-priority MPU region (region 0) as a 4GB
Strongly-ordered, no-access, Execute-Never catch-all, implementing the workaround
for Arm Cortex-M7 erratum 1013783 (SDEN-1068427) and preventing Cortex-M7
speculative accesses to unmapped addresses. Cortex-M7 boards or SoCs whose static
MPU region table explicitly covers all memory the firmware uses can enable it; the
static regions are then programmed starting from MPU region 1. The
``mimxrt1180_evk`` and ``frdm_imxrt1186`` cm7 targets enable it by default,
replacing their previous hand-rolled ``UNMAPPED`` MPU region table entry with
identical runtime behavior.
