/*
 * Copyright (c) 2026 Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/workqs/priority.h>
#include "internal.h"

void workq_prio_enqueue(struct workq_engine *wq, struct work_base *work)
{
	struct workq_prio_work *item = CONTAINER_OF(work, struct workq_prio_work, base);
	struct workq_prio_work *next;
	sys_snode_t *node, *prev = NULL;

	SYS_SLIST_FOR_EACH_NODE(&wq->pending, node) {
		next = CONTAINER_OF(node, struct workq_prio_work, base.node);
		if (item->prio < next->prio) {
			sys_slist_insert(&wq->pending, prev, &work->node);
			return;
		}
		prev = node;
	}

	sys_slist_append(&wq->pending, &work->node);
}

int workq_prio_submit(struct workq_prio *wq, struct workq_prio_work *work, uint8_t prio)
{
	int rc = 0;

	K_SPINLOCK(&wq->engine.lock) {
		rc = z_workq_submit_guard(&wq->engine, &work->base);
		if (rc != 0) {
			K_SPINLOCK_BREAK;
		}
		work->prio = prio;
		z_workq_schedule(&wq->engine, &work->base, K_NO_WAIT);
	}

	return rc;
}

int workq_prio_delayed_submit(struct workq_prio *wq, struct workq_prio_work *work, uint8_t prio,
			      k_timeout_t delay)
{
	int rc = 0;

	K_SPINLOCK(&wq->engine.lock) {
		rc = z_workq_submit_guard(&wq->engine, &work->base);
		if (rc != 0) {
			K_SPINLOCK_BREAK;
		}
		work->prio = prio;
		z_workq_schedule(&wq->engine, &work->base, delay);
	}

	return rc;
}

int workq_prio_reschedule(struct workq_prio *wq, struct workq_prio_work *work, uint8_t prio,
			  k_timeout_t delay)
{
	int rc = 0;

	K_SPINLOCK(&wq->engine.lock) {
		rc = z_workq_reschedule_guard(&wq->engine, &work->base);
		if (rc != 0) {
			K_SPINLOCK_BREAK;
		}
		work->prio = prio;
		z_workq_schedule(&wq->engine, &work->base, delay);
	}

	return rc;
}
