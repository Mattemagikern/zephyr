# Copyright (c) 2026 Måns Ansgariusson <mansgariusson@gmail.com>
# SPDX-License-Identifier: Apache-2.0

zephyr_syscall_header(${ZEPHYR_BASE}/include/zephyr/call_offloader/call_offloader.h)

target_sources(kernel PRIVATE
  engine.c
  )

zephyr_linker_sources(DATA_SECTIONS call_offloader.ld)
zephyr_iterable_section(NAME call_offloader_engine GROUP DATA_REGION ${XIP_ALIGN_WITH_INPUT})
