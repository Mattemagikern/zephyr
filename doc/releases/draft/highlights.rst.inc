**New architectures: Infineon TriCore and Qualcomm Hexagon**
  Zephyr now supports the :zephyr:board-catalog:`Infineon TriCore architecture <#arch=tricore>`,
  used in AURIX automotive microcontrollers, and the
  :zephyr:board-catalog:`Qualcomm Hexagon DSP architecture <#arch=hexagon>`.

**Multimedia Pipeline (mpipe)**
  The new experimental :ref:`Multimedia Pipeline (mpipe) <mpipe>` subsystem, inspired by
  GStreamer, provides a framework for building media applications from source, transform and
  sink elements linked into a zero-copy pipeline.

**Networking enhancements**
  Optimizations across the IP stack deliver up to 21% higher TCP throughput in the
  :ref:`zperf loopback benchmark <zperf-loopback-icount>`.

  The networking stack also gains several new features:

  - :ref:`QUIC <quic_transport_interface>` and :ref:`HTTP/3 <http_server_interface>` support.

  - :ref:`RTP <rtp_interface>` (:rfc:`3550`) stack for sending and receiving media streams over
    UDP.

  - :zephyr:code-sample:`SSH server and client <ssh-server-client>`, including a shell backend.

  - :ref:`Model Context Protocol (MCP) server <mcp_server_interface>` to let AI agents use tools
    exposed by a device.

  - Gateway features: IPv4 forwarding, NAT, IPv6 router role and :ref:`DHCPv6 <dhcpv6_interface>`
    prefix delegation.

**New driver classes**
  Zephyr 4.5 adds several new driver APIs:

  - :ref:`Buzzer <buzzer_api>` for passive and active buzzers driven by PWM or GPIO,

  - :ref:`Clock Monitor <clock_monitor_api>` for runtime observation of clock frequency,

  - :ref:`DALI <dali_api>` for the Digital Addressable Lighting Interface,

  - :ref:`LIN <lin>` for the Local Interconnect Network automotive serial bus,

  - :ref:`MUX <mux_api>` for hardware signal multiplexers, and

  - :ref:`Pulse IO <pulse_io_api>` for hardware that generates and captures timed digital edges,
    as needed by addressable LED strips and infrared transceivers.

**Security and SBOM improvements**
  This release improves SBOM generation, hardening checks and cryptographic acceleration:

  - :ref:`SPDX 3 SBOMs <west-spdx>` that record how the image was built (:ref:`Build profile
    <west-spdx-build-profile>`), the :ref:`prebuilt libraries <west-spdx-blobs>` it links, and
    :ref:`package identifiers <west-spdx-package-id>` for vulnerability matching.

  - A revamped :ref:`hardening tool <hardening>` with CWE references and CI integration.

  - Hardware-accelerated :ref:`PSA Crypto <psa_crypto>` on selected Silicon Labs, NXP and STM32
    devices.

**Simulation and testing improvements**
  This release extends simulated targets and test tooling:

  - New host-backed :ref:`native_sim peripherals <native_sim_peripherals>`, including Wi-Fi, audio,
    video, LEDs and host directory mounts.

  - New :ref:`VirtIO <virtio>` drivers (block, input, GPIO, I2C and SPI) for QEMU targets.

  - :ref:`Multi-device testing <twister_multi_duts_testing>` and
    :ref:`multi-toolchain builds <twister_toolchain_selection>` in Twister.

  - :ref:`TTCN-3 conformance test suites <ttcn3_testing>` for the networking stack.

**Debugging and diagnostics**
  This release adds several debugging and diagnostic tools:

  - :ref:`ZASSERT() <assert>` for per-module and per-file assertion levels.

  - Heap memory error detection with :kconfig:option:`CONFIG_SYS_HEAP_SANITIZER_ASAN` (native_sim)
    and :kconfig:option:`CONFIG_SYS_HEAP_KASAN` (real targets).

  - New :ref:`tracing <tracing>` features: terminal viewer, Perfetto export, shell and ITM backend.

**Expanded board support**
  This release adds support for 178 :ref:`new boards <boards_added_in_zephyr_4_5>` and 29
  :ref:`new shields <shields_added_in_zephyr_4_5>`.
