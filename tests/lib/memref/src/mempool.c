/*
 * Copyright (c) 2025, Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/ztest.h>
#include <zephyr/sys/memref.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(memref_mempool, LOG_LEVEL_DBG);
ZTEST_SUITE(memref_mempool, NULL, NULL, NULL, NULL, NULL);

K_HEAP_DEFINE(mempool, 4096);

void *k_heap_alloc_wrapper(void *ctx, size_t size)
{
	return k_heap_alloc((struct k_heap *)ctx, size, K_NO_WAIT);
}

void k_heap_free_wrapper(void *ctx, void *mem)
{
	k_heap_free((struct k_heap *)ctx, mem);
}

MEMREF_BACKEND(mempool_backend, k_heap_alloc_wrapper, k_heap_free_wrapper, &mempool);

struct container {
	struct k_sem *sem;
};

static void container_cleanup(void *ptr)
{
	struct container *mem = ptr;

	k_sem_give(mem->sem);
}

ZTEST(memref_mempool, basic)
{
	void *mem;

	mem = memref_alloc(&mempool_backend, sizeof(mem), NULL);
	zassert_not_null(mem, "memref_alloc failed");
	memref_unref(&mempool_backend, mem);
}
ZTEST(memref_mempool, cleanup_called)
{
	struct container *mem;
	struct k_sem sem;

	k_sem_init(&sem, 0, 1);

	mem = memref_alloc(&mempool_backend, sizeof(mem), container_cleanup);
	zassert_not_null(mem, "memref_alloc failed");
	mem->sem = &sem;
	memref_unref(&mempool_backend, mem);
	zassert_ok(k_sem_take(&sem, K_MSEC(100)), "cleanup not called");
}

ZTEST(memref_mempool, multi_owners)
{
	struct container *mem;
	struct k_sem sem;

	k_sem_init(&sem, 0, 1);

	mem = memref_alloc(&mempool_backend, sizeof(mem), container_cleanup);
	zassert_not_null(mem, "memref_alloc failed");
	mem->sem = &sem;
	memref_ref(&mempool_backend, mem);
	memref_unref(&mempool_backend, mem);
	zassert_ok(!k_sem_take(&sem, K_MSEC(100)), "cleanup called when refcount > 0");
	memref_unref(&mempool_backend, mem);
	zassert_ok(k_sem_take(&sem, K_MSEC(100)), "cleanup not called");
}

ZTEST(memref_mempool, calloc_zeroed)
{
	uint8_t *mem;

	mem = memref_calloc(&mempool_backend, 8, 12, NULL);
	zassert_not_null(mem, "memref_alloc failed");
	for (size_t i = 0; i < 8*12; i++) {
		zassert_equal(mem[i], 0, "[%d] mem not zeroed", i);
	}
	memref_unref(&mempool_backend, mem);
}

ZTEST(memref_mempool, stress_allocs)
{
	void *mem;
	const size_t alloc_size = 64;
	const size_t iterations = 100;

	for (size_t i = 0; i < iterations; i++) {
		mem = memref_alloc(&mempool_backend, alloc_size, NULL);
		zassert_not_null(mem, "memref_alloc failed");
		memref_unref(&mempool_backend, mem);
	}
}
