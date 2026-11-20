/*
 * Copyright (c) 2026 Måns Ansgariusson <mansgariusson@gmail.com>
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/ztest.h>
#include <zephyr/ztest_error_hook.h>
#include <zephyr/kernel.h>
#include <zephyr/call_offloader/call_offloader.h>

#define USER_STACK_SZ 1024

CALL_OFFLOADER_ENGINE_DEFINE(user_eng, 1, 1024);

static K_THREAD_STACK_DEFINE(user_stack, USER_STACK_SZ);
static struct k_thread user_thread;

ZTEST_BMEM static volatile bool done_returned;

static void forged_done_entry(void *p1, void *p2, void *p3)
{
	ARGS_UNUSED(p1, p2, p3);

	ztest_set_fault_valid(true);
	z_call_offloader_done();
	done_returned = true;
}

static void store_value(void *args)
{
	*(int *)args = 42;
}

ZTEST(call_offloader_userspace, test_done_from_a_foreign_user_thread_oopses)
{
	k_tid_t tid;
	int value = 0;

	done_returned = false;

	tid = k_thread_create(&user_thread, user_stack, K_THREAD_STACK_SIZEOF(user_stack),
			      forged_done_entry, NULL, NULL, NULL, K_PRIO_PREEMPT(1), K_USER,
			      K_NO_WAIT);

	zassert_ok(k_thread_join(tid, K_SECONDS(1)), "the user thread did not terminate");
	zassert_false(done_returned, "the completion syscall returned to a foreign thread");

	call_offloader_dispatch(&user_eng, store_value, &value);
	zassert_equal(value, 42, "the engine did not serve a call after the rejected syscall");
}

ZTEST_SUITE(call_offloader_userspace, NULL, NULL, NULL, NULL, NULL);
