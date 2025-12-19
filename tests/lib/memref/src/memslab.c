/*
 * Copyright (c) 2025, Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/ztest.h>
#include <zephyr/sys/memref.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(memref_memslab, LOG_LEVEL_DBG);
ZTEST_SUITE(memref_memslab, NULL, NULL, NULL, NULL, NULL);

K_MEM_SLAB_DEFINE(memslab, 128, 10, 4);

void *memslab_alloc_wrapper(void *ctx, size_t size)
{
	void *mem;

	if (k_mem_slab_alloc(ctx, &mem, K_NO_WAIT)) {
		return NULL;
	}

	return mem;
}

void memslab_free_wrapper(void *ctx, void *mem)
{
	k_mem_slab_free(ctx, mem);
}

MEMREF_BACKEND(memslab_backend, memslab_alloc_wrapper, memslab_free_wrapper, &memslab);

struct container {
	struct k_sem *sem;
};

static void container_cleanup(void *ptr)
{
	struct container *mem = ptr;

	k_sem_give(mem->sem);
}

ZTEST(memref_memslab, basic)
{
	void *mem;

	mem = memref_alloc(&memslab_backend, sizeof(mem), NULL);
	zassert_not_null(mem, "memref_alloc failed");
	memref_unref(&memslab_backend, mem);
}
ZTEST(memref_memslab, cleanup_called)
{
	struct container *mem;
	struct k_sem sem;

	k_sem_init(&sem, 0, 1);

	mem = memref_alloc(&memslab_backend, sizeof(mem), container_cleanup);
	zassert_not_null(mem, "memref_alloc failed");
	mem->sem = &sem;
	memref_unref(&memslab_backend, mem);
	zassert_ok(k_sem_take(&sem, K_MSEC(100)), "cleanup not called");
}

ZTEST(memref_memslab, multi_owners)
{
	struct container *mem;
	struct k_sem sem;

	k_sem_init(&sem, 0, 1);

	mem = memref_alloc(&memslab_backend, sizeof(mem), container_cleanup);
	zassert_not_null(mem, "memref_alloc failed");
	mem->sem = &sem;
	memref_ref(&memslab_backend, mem);
	memref_unref(&memslab_backend, mem);
	zassert_ok(!k_sem_take(&sem, K_MSEC(100)), "cleanup called when refcount > 0");
	memref_unref(&memslab_backend, mem);
	zassert_ok(k_sem_take(&sem, K_MSEC(100)), "cleanup not called");
}

ZTEST(memref_memslab, calloc_zeroed)
{
	uint8_t *mem;

	mem = memref_calloc(&memslab_backend, 8, 12, NULL);
	zassert_not_null(mem, "memref_alloc failed");
	LOG_DBG("mem allocated at %p", mem);
	for (size_t i = 0; i < 8*12; i++) {
		zassert_equal(mem[i], 0, "[%d] mem not zeroed", i);
	}
	memref_unref(&memslab_backend, mem);
}

ZTEST(memref_memslab, stress_allocs)
{
	void *mem;
	const size_t alloc_size = 64;
	const size_t iterations = 100;

	for (size_t i = 0; i < iterations; i++) {
		mem = memref_alloc(&memslab_backend, alloc_size, NULL);
		zassert_not_null(mem, "memref_alloc failed");
		memref_unref(&memslab_backend, mem);
	}
}
