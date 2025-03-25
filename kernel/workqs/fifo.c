/*
 * Copyright (c) 2026 Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/workqs/fifo.h>
#include "internal.h"

void workq_fifo_enqueue(struct workq_engine *wq, struct work_base *work)
{
	sys_slist_append(&wq->pending, &work->node);
}

int workq_fifo_submit(struct workq_fifo *wq, struct workq_fifo_work *work)
{
	int rc = 0;

	K_SPINLOCK(&wq->engine.lock) {
		rc = z_workq_submit_guard(&wq->engine, &work->base);
		if (rc != 0) {
			K_SPINLOCK_BREAK;
		}
		workq_fifo_enqueue(&wq->engine, &work->base);
		z_workq_awake(&wq->engine);
	}

	return rc;
}

int workq_fifo_delayed_submit(struct workq_fifo *wq, struct workq_fifo_work *work,
			      k_timeout_t delay)
{
	int rc = 0;
	k_timepoint_t exec_time = sys_timepoint_calc(delay);

	K_SPINLOCK(&wq->engine.lock) {
		rc = z_workq_submit_guard(&wq->engine, &work->base);
		if (rc != 0) {
			K_SPINLOCK_BREAK;
		}
		z_workq_delayed_insert(&wq->engine, &work->base, exec_time);
	}

	return rc;
}

int workq_fifo_reschedule(struct workq_fifo *wq, struct workq_fifo_work *work, k_timeout_t delay)
{
	int rc = 0;
	k_timepoint_t exec_time = sys_timepoint_calc(delay);

	K_SPINLOCK(&wq->engine.lock) {
		rc = z_workq_reschedule_guard(&wq->engine, &work->base);
		if (rc != 0) {
			K_SPINLOCK_BREAK;
		}
		z_workq_delayed_insert(&wq->engine, &work->base, exec_time);
	}

	return rc;
}
