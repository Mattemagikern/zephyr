/*
 * Copyright (c) 2025 Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/kernel.h>
#include <zephyr/workq.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(worq_sample, LOG_LEVEL_DBG);

#define PRIORITY 0

WORKQ_DEFINE(my_workq);
WORKQ_THREAD_DEFINE(thread1, my_workq, 1024, PRIORITY);
WORKQ_THREAD_DEFINE(thread2, my_workq, 1024, PRIORITY);

struct container {
	struct work_item item;
	bool delayable;
	size_t number;
};

static void work_fn(struct work_item *item)
{
	struct container *c = CONTAINER_OF(item, struct container, item);

	if (c->delayable) {
		LOG_WRN("[%p] Delayed work(%p) executing:%d", k_current_get(), c, c->number);
	} else {
		LOG_INF("[%p] Work(%p) executing:%d", k_current_get(), c, c->number);
	}
	k_msleep(100);
	if (c->delayable) {
		LOG_WRN("[%p] Delayed work(%p) executed:%d", k_current_get(), c, c->number);
	} else {
		LOG_INF("[%p] Work(%p) executed :%d", k_current_get(), c, c->number);
	}
	k_free(c);
}

int main(void)
{
	uint32_t delay;
	struct container *c;

	LOG_INF("Hello from Zephyr Work Queue example!");
	for (size_t i = 0; i < 5; i++) {
		c = k_malloc(sizeof(struct container));
		__ASSERT(c != NULL, "Memory allocation failed");
		work_init(&c->item, work_fn);
		c->delayable = true;
		delay = 5 + i * 5;
		c->number = i;
		LOG_DBG("[%p] Submitting delayed(%d ms) work(%p) item:%d",
				k_current_get(), delay, c, c->number);
		workq_delayed_submit(&my_workq, &c->item, K_MSEC(delay));
	}

	for (size_t i = 0; i < 5; i++) {
		c = k_malloc(sizeof(struct container));
		__ASSERT(c != NULL, "Memory allocation failed");
		work_init(&c->item, work_fn);
		c->delayable = false;
		c->number = i+5;
		LOG_DBG("[%p] Submitting work(%p) item:%d", k_current_get(), c, c->number);
		workq_submit(&my_workq, &c->item);
		k_msleep(10);
	}

	workq_drain(&my_workq, K_FOREVER);
	LOG_INF("All work items have been processed");
	return 0;
}
