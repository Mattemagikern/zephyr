/*
 * Copyright (c) 2026 Måns Ansgariusson <mansgariusson@gmail.com>
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/ztest.h>
#include <zephyr/kernel.h>
#include <zephyr/call_offloader/call_offloader.h>

#define WRAP_WORKER_STACK_SZ 1024

CALL_OFFLOADER_ENGINE_DEFINE(wrap_eng, 1, WRAP_WORKER_STACK_SZ);

static k_tid_t wrap_tid;
static int wrap_prio;
static unsigned int wrap_calls;

static void record_caller(void)
{
	wrap_tid = k_current_get();
	wrap_prio = k_thread_priority_get(wrap_tid);
}

CALL_OFFLOADER_WRAP(wrap_eng, unsigned int, count_calls)
{
	record_caller();

	return ++wrap_calls;
}

CALL_OFFLOADER_WRAP(wrap_eng, int, sum_and_store,
		    (int) a,
		    (const int *) b,
		    (size_t) n,
		    (int *) out)
{
	int sum = a;

	record_caller();

	for (size_t i = 0; i < n; i++) {
		sum += b[i];
	}

	*out = sum;

	return -sum;
}

static int scale_value(int value, int factor)
{
	return value * factor;
}

CALL_OFFLOADER_WRAP_API(wrap_eng, int, scale_value, (int) value, (int) factor)
{
	record_caller();

	return scale_value(value, factor);
}

static void before(void *fixture)
{
	ARG_UNUSED(fixture);

	wrap_tid = NULL;
	wrap_prio = K_LOWEST_APPLICATION_THREAD_PRIO;
	wrap_calls = 0;
}

ZTEST(call_offloader_wrap, test_wrapper_without_arguments)
{
	zassert_equal(call_offloader_count_calls(), 1, "the wrapper returned the wrong value");
	zassert_equal(call_offloader_count_calls(), 2, "the body did not run a second time");
	zassert_not_null(wrap_tid, "the body did not run");
	zassert_true(wrap_tid != k_current_get(), "the body ran on the caller");
}

ZTEST(call_offloader_wrap, test_wrapper_forwards_every_argument)
{
	const int values[] = {2, 3, 4};
	int out = 0;
	int ret;

	ret = call_offloader_sum_and_store(1, values, ARRAY_SIZE(values), &out);

	zassert_equal(out, 10, "an argument was not forwarded, got %d", out);
	zassert_equal(ret, -10, "the return value was not propagated, got %d", ret);
	zassert_true(wrap_tid != k_current_get(), "the body ran on the caller");
}

ZTEST(call_offloader_wrap, test_api_wrapper_matches_the_wrapped_function)
{
	int ret = call_offloader_scale_value(6, 7);

	zassert_equal(ret, 42, "the wrapped function returned %d", ret);
	zassert_not_null(wrap_tid, "the body did not run");
	zassert_true(wrap_tid != k_current_get(), "the body ran on the caller");
}

ZTEST(call_offloader_wrap, test_wrapper_runs_at_the_caller_priority)
{
	int prio = k_thread_priority_get(k_current_get());

	(void)call_offloader_count_calls();

	zassert_equal(wrap_prio, prio, "the body ran at %d, expected the caller priority %d",
		      wrap_prio, prio);
}

ZTEST_SUITE(call_offloader_wrap, NULL, NULL, before, NULL, NULL);
