/*
 * Copyright (c) 2025, Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/ztest.h>
#include <zephyr/sys/memref.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(memref_sys_heap, LOG_LEVEL_DBG);
ZTEST_SUITE(memref_sys_heap, NULL, NULL, NULL, NULL, NULL);

void *k_malloc_wrapper(void *ctx, size_t size)
{
	(void)ctx;

	return k_malloc(size);
}

void k_free_wrapper(void *ctx, void *mem)
{
	(void)ctx;

	k_free(mem);
}

MEMREF_BACKEND(sys_heap, k_malloc_wrapper, k_free_wrapper, NULL);

struct container {
	struct k_sem *sem;
};

static void container_cleanup(void *ptr)
{
	struct container *mem = ptr;

	k_sem_give(mem->sem);
}

ZTEST(memref_sys_heap, basic)
{
	void *mem;

	mem = memref_alloc(&sys_heap, sizeof(mem), NULL);
	zassert_not_null(mem, "memref_alloc failed");
	memref_unref(&sys_heap, mem);
}
ZTEST(memref_sys_heap, cleanup_called)
{
	struct container *mem;
	struct k_sem sem;

	k_sem_init(&sem, 0, 1);

	mem = memref_alloc(&sys_heap, sizeof(mem), container_cleanup);
	zassert_not_null(mem, "memref_alloc failed");
	mem->sem = &sem;
	memref_unref(&sys_heap, mem);
	zassert_ok(k_sem_take(&sem, K_MSEC(100)), "cleanup not called");
}

ZTEST(memref_sys_heap, multi_owners)
{
	struct container *mem;
	struct k_sem sem;

	k_sem_init(&sem, 0, 1);

	mem = memref_alloc(&sys_heap, sizeof(mem), container_cleanup);
	zassert_not_null(mem, "memref_alloc failed");
	mem->sem = &sem;
	memref_ref(&sys_heap, mem);
	memref_unref(&sys_heap, mem);
	zassert_ok(!k_sem_take(&sem, K_MSEC(100)), "cleanup called when refcount > 0");
	memref_unref(&sys_heap, mem);
	zassert_ok(k_sem_take(&sem, K_MSEC(100)), "cleanup not called");
}

ZTEST(memref_sys_heap, calloc_zeroed)
{
	uint8_t *mem;

	mem = memref_calloc(&sys_heap, 8, 12, NULL);
	zassert_not_null(mem, "memref_alloc failed");
	LOG_DBG("mem allocated at %p", mem);
	for (size_t i = 0; i < 8*12; i++) {
		zassert_equal(mem[i], 0, "[%d] mem not zeroed", i);
	}
	memref_unref(&sys_heap, mem);
}

ZTEST(memref_sys_heap, stress_allocs)
{
	void *mem;
	const size_t alloc_size = 64;
	const size_t iterations = 100;

	for (size_t i = 0; i < iterations; i++) {
		mem = memref_alloc(&sys_heap, alloc_size, NULL);
		zassert_not_null(mem, "memref_alloc failed");
		memref_unref(&sys_heap, mem);
	}
}
