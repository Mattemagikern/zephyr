/*
 * Copyright (c) 2026 Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/ztest.h>
#include <zephyr/workqs/priority.h>
#include <string.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(workq_prio_tests, LOG_LEVEL_DBG);

#define WORKQ_TEST_STACK_SIZE 2048

WORKQ_PRIO_DEFINE(test_define_workq);
WORKQ_PRIO_THREAD_DEFINE(test_define_wqt, test_define_workq, WORKQ_TEST_STACK_SIZE, 0);

struct item {
	struct workq_prio_work work;
	int id;
};

static int order_log[16];
static size_t order_idx;

static void reset_log(void)
{
	order_idx = 0;
	memset(order_log, 0, sizeof(order_log));
}

static void record_fn(struct work_base *base)
{
	struct workq_prio_work *w = CONTAINER_OF(base, struct workq_prio_work, base);
	struct item *it = CONTAINER_OF(w, struct item, work);

	order_log[order_idx++] = it->id;
}

static void drain_on_thread(struct workq_prio *q, size_t count)
{
	for (size_t i = 0; i < count; i++) {
		zassert_ok(workq_prio_run(q, K_NO_WAIT), "workq_prio_run failed");
	}
}

ZTEST(priority, test_macro_defined_workq)
{
	zassert_true(test_define_workq.engine.flags == WORKQ_FLAG_OPEN, "workq should be open");
	zassert_true(test_define_wqt.flags & WORKQ_THREAD_FLAG_INITIALIZED,
			"workq thread should be initialized");
	zassert_true(test_define_wqt.flags & WORKQ_THREAD_FLAG_RUNNING,
			"workq thread should be running");
	zassert_true(workq_prio_run(&test_define_workq, K_NO_WAIT) == -EAGAIN,
			"workq have no work, should return -EAGAIN");

	zassert_true(workq_prio_thread_start(&test_define_wqt) == -EALREADY,
			"workq thread already started, should return -EALREADY");
	zassert_true(workq_prio_thread_stop(&test_define_wqt, K_MSEC(10)) == 0,
			"workq thread should stop");
}

ZTEST(priority, test_submit_orders_by_priority)
{
	struct workq_prio q;
	struct item items[4] = {
		{ .id = 0 }, { .id = 1 }, { .id = 2 }, { .id = 3 },
	};
	const uint8_t prios[4] = { 3, 1, 2, 0 };
	const int expect[4] = { 3, 1, 2, 0 };

	reset_log();
	workq_prio_init(&q);

	for (size_t i = 0; i < 4; i++) {
		workq_prio_work_init(&items[i].work, record_fn);
		zassert_ok(workq_prio_submit(&q, &items[i].work, prios[i]),
				"workq_prio_submit failed");
	}

	drain_on_thread(&q, 4);

	zassert_mem_equal(order_log, expect, sizeof(expect),
			"items did not run in ascending priority order");
}

ZTEST(priority, test_equal_priority_is_fifo)
{
	struct workq_prio q;
	struct item items[3] = { { .id = 0 }, { .id = 1 }, { .id = 2 } };
	const int expect[3] = { 0, 1, 2 };

	reset_log();
	workq_prio_init(&q);

	for (size_t i = 0; i < 3; i++) {
		workq_prio_work_init(&items[i].work, record_fn);
		zassert_ok(workq_prio_submit(&q, &items[i].work, 5), "workq_prio_submit failed");
	}

	drain_on_thread(&q, 3);

	zassert_mem_equal(order_log, expect, sizeof(expect),
			"equal-priority items did not run in submission order");
}

ZTEST(priority, test_high_prio_jumps_pending_queue)
{
	struct workq_prio q;
	struct item items[3] = { { .id = 0 }, { .id = 1 }, { .id = 2 } };
	const int expect[3] = { 2, 0, 1 };

	reset_log();
	workq_prio_init(&q);

	workq_prio_work_init(&items[0].work, record_fn);
	workq_prio_work_init(&items[1].work, record_fn);
	workq_prio_work_init(&items[2].work, record_fn);

	zassert_ok(workq_prio_submit(&q, &items[0].work, 5), "workq_prio_submit failed");
	zassert_ok(workq_prio_submit(&q, &items[1].work, 5), "workq_prio_submit failed");
	/* A late high-priority item must run before the already-pending ones. */
	zassert_ok(workq_prio_submit(&q, &items[2].work, 0), "workq_prio_submit failed");

	drain_on_thread(&q, 3);

	zassert_mem_equal(order_log, expect, sizeof(expect),
			"high-priority item did not jump the pending queue");
}

ZTEST(priority, test_delayed_promotion_orders_by_priority)
{
	struct workq_prio q;
	struct item items[3] = { { .id = 0 }, { .id = 1 }, { .id = 2 } };
	const uint8_t prios[3] = { 2, 0, 1 };
	const int expect[3] = { 1, 2, 0 };

	reset_log();
	workq_prio_init(&q);

	/* Same delay, different priorities: promotion must sort by priority. */
	for (size_t i = 0; i < 3; i++) {
		workq_prio_work_init(&items[i].work, record_fn);
		zassert_ok(workq_prio_delayed_submit(&q, &items[i].work, prios[i], K_MSEC(20)),
				"workq_prio_delayed_submit failed");
	}

	k_msleep(50); /* let the delayed timeout fire and promote all three */

	drain_on_thread(&q, 3);

	zassert_mem_equal(order_log, expect, sizeof(expect),
			"delayed items were not promoted in priority order");
}

ZTEST(priority, test_prio_boundaries)
{
	struct workq_prio q;
	struct item items[2] = { { .id = 0 }, { .id = 1 } };
	const int expect[2] = { 1, 0 };

	reset_log();
	workq_prio_init(&q);

	workq_prio_work_init(&items[0].work, record_fn);
	workq_prio_work_init(&items[1].work, record_fn);

	zassert_ok(workq_prio_submit(&q, &items[0].work, UINT8_MAX), "workq_prio_submit failed");
	zassert_ok(workq_prio_submit(&q, &items[1].work, 0), "workq_prio_submit failed");

	drain_on_thread(&q, 2);

	zassert_mem_equal(order_log, expect, sizeof(expect),
			"priority boundaries not ordered correctly");
}

ZTEST(priority, test_reschedule_changes_priority)
{
	struct workq_prio q;
	struct item items[2] = { { .id = 0 }, { .id = 1 } };
	const int expect[2] = { 1, 0 };

	reset_log();
	workq_prio_init(&q);

	workq_prio_work_init(&items[0].work, record_fn);
	workq_prio_work_init(&items[1].work, record_fn);

	/* id0 starts ahead of id1 ... */
	zassert_ok(workq_prio_submit(&q, &items[0].work, 0), "workq_prio_submit failed");
	zassert_ok(workq_prio_submit(&q, &items[1].work, 1), "workq_prio_submit failed");

	/* ... but rescheduling it to a lower priority moves it behind id1. */
	zassert_ok(workq_prio_reschedule(&q, &items[0].work, 2, K_NO_WAIT),
			"workq_prio_reschedule failed");
	k_msleep(10); /* let the (immediate) reschedule promote back to pending */

	drain_on_thread(&q, 2);

	zassert_mem_equal(order_log, expect, sizeof(expect),
			"reschedule did not change the item's priority position");
}

ZTEST(priority, test_submit_already_preserves_order)
{
	struct workq_prio q;
	struct item items[3] = { { .id = 0 }, { .id = 1 }, { .id = 2 } };
	const int expect[3] = { 2, 0, 1 };

	reset_log();
	workq_prio_init(&q);

	workq_prio_work_init(&items[0].work, record_fn);
	workq_prio_work_init(&items[1].work, record_fn);
	workq_prio_work_init(&items[2].work, record_fn);

	zassert_ok(workq_prio_submit(&q, &items[0].work, 5), "workq_prio_submit failed");
	zassert_ok(workq_prio_submit(&q, &items[1].work, 5), "workq_prio_submit failed");

	/*
	 * Re-submitting a pending item must return -EALREADY without mutating its
	 * stored priority; otherwise the pending list order would silently drift.
	 */
	zassert_equal(-EALREADY, workq_prio_submit(&q, &items[0].work, 0),
			"resubmit of a pending item should return -EALREADY");

	/* id2 must land ahead of both prio-5 items, not between them. */
	zassert_ok(workq_prio_submit(&q, &items[2].work, 1), "workq_prio_submit failed");

	drain_on_thread(&q, 3);

	zassert_mem_equal(order_log, expect, sizeof(expect),
			"-EALREADY submit corrupted the pending order");
}

ZTEST_SUITE(priority, NULL, NULL, ztest_simple_1cpu_before,
	    ztest_simple_1cpu_after, NULL);
