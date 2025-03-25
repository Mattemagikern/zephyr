/*
 * Copyright (c) 2025 Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/ztest.h>
#include <zephyr/workq.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(workq_tests, LOG_LEVEL_DBG);
WORKQ_DEFINE(test_define_workq);
WORKQ_THREAD_DEFINE(test_define_wqt, test_define_workq, 1024, 0);

struct container {
	struct work_item item;
	struct k_sem *sem;
	int count;
};

static void work_fn(struct work_item *item)
{
	struct container *c = CONTAINER_OF(item, struct container, item);

	k_sem_give(c->sem);
	k_free(c);
}

static void work_fn_locking(struct work_item *item)
{
	struct container *c = CONTAINER_OF(item, struct container, item);

	k_sem_take(c->sem, K_FOREVER);
	k_free(c);
}

ZTEST(basic, test_macro_defined_workq)
{
	zassert_true(test_define_workq.flags == WORKQ_FLAG_OPEN, "workq should be open");
	zassert_true(test_define_wqt.flags & WORKQ_THREAD_FLAG_INITIALIZED,
			"workq thread should be initialized");
	zassert_true(test_define_wqt.flags & WORKQ_THREAD_FLAG_RUNNING,
			"workq thread should be running");
	zassert_true(workq_run(&test_define_workq, K_NO_WAIT) == -EAGAIN,
			"workq have no work, should return -EAGAIN");

	zassert_true(workq_thread_start(&test_define_wqt) == -EALREADY,
			"workq thread already started, should return -EALREADY");
	zassert_true(workq_thread_stop(&test_define_wqt, K_MSEC(10)) == 0,
			"workq thread should stop");
}

ZTEST(basic, test_setup)
{
	struct workq q;

	LOG_INF("----------------------------");
	LOG_INF("sizeof k_work %d", sizeof(struct k_work));
	LOG_INF("sizeof k_work_delayable %d", sizeof(struct k_work_delayable));
	LOG_INF("sizeof k_workq %d", sizeof(struct k_work_q));
	LOG_INF("----------------------------");
	LOG_INF("sizeof work %d", sizeof(struct work_item));
	LOG_INF("sizeof q %d", sizeof(struct workq));
	LOG_INF("sizeof wqt %d", sizeof(struct workq_thread));
	LOG_INF("----------------------------");

	workq_init(&q);
	zassert_true(workq_run(&q, K_NO_WAIT) == -EAGAIN,
			"workq have no work, should return -EAGAIN");
}

ZTEST(basic, test_submit)
{
	struct workq q;
	struct k_sem sem;
	struct container *c;

	k_sem_init(&sem, 0, 3);
	workq_init(&q);

	for (size_t i = 0; i < 3; i++) {
		c = k_malloc(sizeof(struct container));
		zassert_not_null(c, "Failed to allocate memory for container");
		c->sem = &sem;
		c->count = i;
		work_init(&c->item, work_fn);
		zassert_true(workq_submit(&q, &c->item) == 0, "workq_submit failed");
	}
	for (size_t i = 0; i < 3; i++) {
		zassert_ok(workq_run(&q, K_NO_WAIT), "workq_run failed");
	}
	for (size_t i = 0; i < 3; i++) {
		zassert_ok(k_sem_take(&sem, K_NO_WAIT), "work_fn not called");
	}
}

ZTEST(basic, test_submit_delayed)
{
	struct workq q;
	struct k_sem sem;
	struct container *c;

	workq_init(&q);
	k_sem_init(&sem, 0, 3);

	for (size_t i = 0; i < 3; i++) {
		c = k_malloc(sizeof(struct container));
		zassert_not_null(c, "Failed to allocate memory for container");
		work_init(&c->item, work_fn);
		c->sem = &sem;
		c->count = i;
		zassert_true(workq_delayed_submit(&q, &c->item, K_MSEC(50 - i*10)) == 0,
				"workq_submit failed");
	}

	for (size_t i = 0; i < 3; i++) {
		zassert_ok(workq_run(&q, K_MSEC(50)), "workq_run failed");
	}

	for (size_t i = 0; i < 3; i++) {
		zassert_ok(k_sem_take(&sem, K_NO_WAIT), "work_fn not called");
	}
}

ZTEST(basic, test_cancel)
{
	struct workq q;
	struct k_sem sem;
	struct container *c;

	workq_init(&q);
	k_sem_init(&sem, 0, 1);

	c = k_malloc(sizeof(struct container));
	zassert_not_null(c, "Failed to allocate memory for container");
	c->sem = &sem;
	work_init(&c->item, work_fn);

	zassert_true(workq_delayed_submit(&q, &c->item, K_MSEC(50)) == 0, "workq_submit failed");
	zassert_ok(workq_cancel(&q, &c->item), "workq_cancel failed");
	zassert_true(workq_run(&q, K_MSEC(100)) == -EAGAIN, "workq_run should timeout");
	zassert_not_ok(k_sem_take(&sem, K_NO_WAIT), "work_fn called?");
}

static void work_fn_nonfree(struct work_item *item)
{
	LOG_DBG("%s called", __func__);
}

K_THREAD_STACK_DEFINE(stack, 1024);
ZTEST(basic, test_flush)
{
	struct workq_thread wqt;
	struct workq q;
	struct work_item item;

	workq_init(&q);
	work_init(&item, work_fn_nonfree);
	workq_thread_init(&wqt, &q, stack, K_THREAD_STACK_SIZEOF(stack), NULL);
	zassert_ok(workq_thread_start(&wqt), "workq_thread_start failed");

	zassert_ok(workq_delayed_submit(&q, &item, K_MSEC(75)), "workq_submit failed");
	zassert_ok(workq_sync(&q, &item, K_MSEC(100)), "workq_flush failed");
	zassert_ok(workq_thread_stop(&wqt, K_MSEC(100)), "Failed to stop workq thread");
}

static void work_fn_long_sleep(struct work_item *item)
{
	LOG_DBG("%s called", __func__);
	k_sleep(K_MSEC(100));
}

ZTEST(basic, test_work_state)
{
	struct workq_thread wqt;
	struct workq q;
	struct work_item item;
	struct work_item item2;

	workq_init(&q);
	work_init(&item, work_fn_nonfree);
	work_init(&item2, work_fn_long_sleep);
	workq_thread_init(&wqt, &q, stack, K_THREAD_STACK_SIZEOF(stack), NULL);

	zassert_true(WORK_STATUS_IDLE == workq_work_status(&q, &item), "workq_submit failed");
	zassert_ok(workq_submit(&q, &item), "workq_submit failed");
	zassert_true(WORK_STATUS_PENDING == workq_work_status(&q, &item), "workq_submit failed");
	zassert_ok(workq_thread_start(&wqt), "workq_thread_start failed");
	zassert_ok(workq_submit(&q, &item2), "workq_submit failed");
	k_sleep(K_MSEC(25));
	zassert_true(WORK_STATUS_RUNNING == workq_work_status(&q, &item2), "workq_submit failed");


	zassert_ok(workq_sync(&q, &item2, K_MSEC(100)), "workq_flush failed");
	zassert_ok(workq_thread_stop(&wqt, K_MSEC(100)), "Failed to stop workq thread");
}

ZTEST(basic, test_sync)
{
	struct workq_thread wqt;
	struct workq q;
	struct work_item item;

	workq_init(&q);
	work_init(&item, work_fn_nonfree);
	workq_thread_init(&wqt, &q, stack, K_THREAD_STACK_SIZEOF(stack), NULL);

	zassert_ok(workq_thread_start(&wqt), "workq_thread_start failed");
	zassert_ok(workq_submit(&q, &item), "workq_submit failed");
	zassert_ok(workq_sync(&q, &item, K_MSEC(100)), "workq_drain failed");

	zassert_ok(workq_thread_stop(&wqt, K_MSEC(100)), "Failed to stop workq thread");
}

ZTEST(basic, test_drain)
{
	struct workq_thread wqt;
	struct workq q;
	struct work_item item;
	struct work_item item2;

	workq_init(&q);
	work_init(&item, work_fn_nonfree);
	work_init(&item2, work_fn_nonfree);
	workq_thread_init(&wqt, &q, stack, K_THREAD_STACK_SIZEOF(stack), NULL);

	zassert_ok(workq_thread_start(&wqt), "workq_thread_start failed");
	zassert_ok(workq_delayed_submit(&q, &item, K_MSEC(500)), "workq_submit failed");
	zassert_true(-EAGAIN == workq_drain(&q, K_MSEC(200)), "workq_drain failed");
	zassert_ok(workq_drain(&q, K_MSEC(350)), "workq_drain failed");

	zassert_ok(workq_thread_stop(&wqt, K_MSEC(100)), "Failed to stop workq thread");
}

ZTEST(basic, test_reschedule)
{
	struct workq_thread wqt;
	struct work_item item;
	struct workq q;

	workq_init(&q);
	work_init(&item, work_fn_nonfree);
	workq_thread_init(&wqt, &q, stack, K_THREAD_STACK_SIZEOF(stack), NULL);

	zassert_ok(workq_thread_start(&wqt), "workq_thread_start failed");
	zassert_ok(workq_delayed_submit(&q, &item, K_MSEC(500)), "workq_submit failed");
	zassert_ok(workq_reschedule(&q, &item, K_MSEC(100)), "workq_reschedule failed");
	zassert_ok(workq_sync(&q, &item, K_MSEC(150)), "workq_flush failed");

	zassert_ok(workq_thread_stop(&wqt, K_MSEC(100)), "Failed to stop workq thread");
}

ZTEST(basic, test_status)
{
	struct workq_thread wqt;
	struct workq q;
	struct k_sem sem;
	struct container *c;

	k_sem_init(&sem, 0, 1);

	c = k_malloc(sizeof(struct container));
	zassert_not_null(c, "Failed to allocate memory for container");
	c->sem = &sem;
	work_init(&c->item, work_fn_locking);

	workq_init(&q);
	workq_thread_init(&wqt, &q, stack, K_THREAD_STACK_SIZEOF(stack), NULL);

	zassert_true(WORK_STATUS_IDLE == workq_work_status(&q, &c->item),
			"workq_work_status failed");
	zassert_ok(workq_submit(&q, &c->item), "workq_submit failed");
	zassert_true(WORK_STATUS_PENDING == workq_work_status(&q, &c->item),
			"workq_work_status failed");
	zassert_ok(workq_thread_start(&wqt), "workq_thread_start failed");
	k_msleep(100);
	zassert_true(WORK_STATUS_RUNNING == workq_work_status(&q, &c->item),
			"workq_work_status failed");
	k_sem_give(&sem);
	zassert_ok(workq_sync(&q, &c->item, K_MSEC(100)), "workq_flush failed");

	zassert_ok(workq_thread_stop(&wqt, K_MSEC(100)), "Failed to stop workq thread");
}

ZTEST(basic, test_open_close)
{
	struct workq_thread wqt;
	struct workq q;
	struct k_sem sem;
	struct container *c;

	k_sem_init(&sem, 0, 1);
	c = k_malloc(sizeof(struct container));
	zassert_not_null(c, "Failed to allocate memory for container");
	c->sem = &sem;
	work_init(&c->item, work_fn);

	workq_init(&q);

	workq_close(&q);
	zassert_true(-EAGAIN == workq_submit(&q, &c->item), "workq_submit should fail when closed");

	workq_open(&q);
	zassert_ok(workq_submit(&q, &c->item), "workq_open failed");

}

ZTEST_SUITE(basic, NULL, NULL, ztest_simple_1cpu_before,
	    ztest_simple_1cpu_after, NULL);
