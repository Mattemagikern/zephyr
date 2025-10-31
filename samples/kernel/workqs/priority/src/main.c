/*
 * Copyright (c) 2026 Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/kernel.h>
#include <zephyr/workqs/priority.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(workq_prio_sample, LOG_LEVEL_DBG);

#define STACK_SIZE 1024
#define PRIORITY   0

WORKQ_PRIO_DEFINE(my_prioq);
K_THREAD_STACK_DEFINE(worker_stack, STACK_SIZE);
static struct workq_thread worker;

struct container {
	struct workq_prio_work item;
	uint8_t prio;
	bool delayable;
};

static void work_fn(struct work_base *base)
{
	struct workq_prio_work *w = CONTAINER_OF(base, struct workq_prio_work, base);
	struct container *c = CONTAINER_OF(w, struct container, item);

	if (c->delayable) {
		LOG_WRN("[%p] Delayed work(%p) prio:%u executing", k_current_get(), c, c->prio);
	} else {
		LOG_INF("[%p] Work(%p) prio:%u executing", k_current_get(), c, c->prio);
	}
	k_msleep(50);
	k_free(c);
}

static struct container *make_work(uint8_t prio, bool delayable)
{
	struct container *c = k_malloc(sizeof(struct container));

	__ASSERT(c != NULL, "Memory allocation failed");
	workq_prio_work_init(&c->item, work_fn);
	c->prio = prio;
	c->delayable = delayable;

	return c;
}

void sample_priority_ordering(void)
{
	const uint8_t prios[] = { 4, 0, 2, 1, 3 };
	struct container *c;

	/*
	 * Queue a batch of mixed-priority work first, then start the worker, so
	 * the execution order reflects the priority ordering rather than the
	 * submission timing.
	 */
	for (size_t i = 0; i < ARRAY_SIZE(prios); i++) {
		c = make_work(prios[i], false);
		LOG_DBG("Submitting work(%p) prio:%u", c, c->prio);
		workq_prio_submit(&my_prioq, &c->item, c->prio);
	}

	workq_prio_thread_init(&worker, &my_prioq, worker_stack,
			       K_THREAD_STACK_SIZEOF(worker_stack), NULL);
	workq_prio_thread_start(&worker);

	workq_prio_drain(&my_prioq, K_FOREVER);
	LOG_INF("All work items have been processed in priority order");
}

void sample_delayed_priority(void)
{
	const uint8_t prios[] = { 2, 0, 1 };
	struct container *c;

	/* Same delay, different priorities: promotion runs highest priority first. */
	for (size_t i = 0; i < ARRAY_SIZE(prios); i++) {
		c = make_work(prios[i], true);
		LOG_DBG("Submitting delayed work(%p) prio:%u", c, c->prio);
		workq_prio_delayed_submit(&my_prioq, &c->item, c->prio, K_MSEC(20));
	}

	workq_prio_drain(&my_prioq, K_FOREVER);
	LOG_INF("All delayed work items have been processed in priority order");
}

int main(void)
{
	LOG_INF("Hello from Zephyr Priority Work Queue example!");
	sample_priority_ordering();
	sample_delayed_priority();
	return 0;
}
