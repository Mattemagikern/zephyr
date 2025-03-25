/*
 * Copyright (c) 2026 Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ZEPHYR_KERNEL_WORKQS_INTERNAL_H_
#define ZEPHYR_KERNEL_WORKQS_INTERNAL_H_

#include <zephyr/workqs/engine.h>

/*
 * Engine primitives shared by the work queue variants. Every function must be
 * called with the engine lock held.
 */

void z_workq_awake(struct workq_engine *wq);

void z_workq_delayed_insert(struct workq_engine *wq, struct work_base *item,
			    k_timepoint_t exec_time);

int z_workq_submit_guard(struct workq_engine *wq, struct work_base *item);

int z_workq_reschedule_guard(struct workq_engine *wq, struct work_base *item);

#endif /* ZEPHYR_KERNEL_WORKQS_INTERNAL_H_ */
