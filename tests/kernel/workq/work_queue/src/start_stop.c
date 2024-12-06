/*
 * Copyright (c) 2024 Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/ztest.h>

#define NUM_TEST_ROUNDS 5
#define NUM_TEST_ITEMS  10
/* Each work item takes 100ms by default. */
#define WORK_ITEM_WAIT  (CONFIG_TEST_WORK_ITEM_WAIT_MS)
/* In fact, each work item could take up to this value */
#define WORK_ITEM_WAIT_ALIGNED                                                                     \
	k_ticks_to_ms_floor64(k_ms_to_ticks_ceil32(WORK_ITEM_WAIT) + _TICK_ALIGN)
#define SUBMIT_WAIT (CONFIG_TEST_SUBMIT_WAIT_MS)
#define STACK_SIZE  (1024 + CONFIG_TEST_EXTRA_STACK_SIZE)
#define CHECK_WAIT  ((NUM_TEST_ITEMS + 1) * WORK_ITEM_WAIT_ALIGNED)

static struct k_work_q work_q;
static K_THREAD_STACK_DEFINE(work_q_stack, STACK_SIZE);

struct k_work works[NUM_TEST_ITEMS];

static atomic_t num_work_executed;
static void work_handler(struct k_work *work)
{
	ARG_UNUSED(work);

	atomic_inc(&num_work_executed);
}

static struct k_work_queue_config cfg = {
	.name = "test_work_q",
	.no_yield = true,
};

static void workq_setup(void)
{
	zassert_equal(k_work_queue_stop(&work_q), -ENODEV,
		      "Succeeded to stop work queue on non-initialized work queue");
	k_work_queue_start(&work_q, work_q_stack, K_THREAD_STACK_SIZEOF(work_q_stack),
			   K_PRIO_PREEMPT(4), &cfg);
}

static void workq_teardown(void)
{
	struct k_work work;

	zassert_equal(k_work_queue_stop(&work_q), -EBUSY,
		      "Succeeded to stop work queue while it is running & not plugged");
	zassert_true(k_work_queue_drain(&work_q, true) >= 0, "Failed to drain & plug work queue");
	zassert_ok(k_work_queue_stop(&work_q), "Failed to stop work queue");

	k_work_init(&work, work_handler);
	zassert_equal(k_work_submit_to_queue(&work_q, &work), -ENODEV,
		      "Succeeded to submit work item to non-initialized work queue");
}

ZTEST(workqueue_reuse, test_submit_to_queue)
{
	size_t i, j;

	for (i = 0; i < NUM_TEST_ROUNDS; i++) {
		workq_setup();
		for (j = 0; j < NUM_TEST_ITEMS; j++) {
			k_work_init(&works[j], work_handler);
			zassert_equal(k_work_submit_to_queue(&work_q, &works[j]), 1,
				      "Failed to submit work item");
		}
		/* Wait for the work item to complete */
		k_sleep(K_MSEC(CHECK_WAIT));

		/* Check that the work item was executed */
		zassert_equal(atomic_get(&num_work_executed), i * j + j,
			      "all work items were not executed within the expected time");
		workq_teardown();
	}
}

ZTEST_SUITE(workqueue_reuse, NULL, NULL, NULL, NULL, NULL);
