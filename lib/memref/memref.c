/*
 * Copyright (c) 2025 Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/kernel.h>
#include <zephyr/sys/memref.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/util.h>
#include <zephyr/types.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(memref, LOG_LEVEL_DBG);

#define MEMREF_CTRL_OFFSET ROUND_UP(sizeof(struct ctrl_block), __alignof__(z_max_align_t))

struct ctrl_block {
	atomic_t ref_count;
	memref_free_cb_t free_cb;
	const struct memref_backend *backend;
};

static ALWAYS_INLINE struct ctrl_block *ctrl_from_mem(void *mem)
{
	return (struct ctrl_block *)((uint8_t *)mem - MEMREF_CTRL_OFFSET);
}

static ALWAYS_INLINE void *mem_from_ctrl(struct ctrl_block *ctrl)
{
	return (void *)((uint8_t *)ctrl + MEMREF_CTRL_OFFSET);
}

void *memref_alloc(const struct memref_backend *be, size_t size, memref_free_cb_t free_cb)
{
	size_t total;
	struct ctrl_block *ctrl;

	__ASSERT_NO_MSG(be != NULL && be->alloc != NULL && be->free != NULL);

	if (unlikely(size_add_overflow(size, MEMREF_CTRL_OFFSET, &total))) {
		return NULL;
	}

	ctrl = be->alloc(be->ctx, total);
	if (!ctrl) {
		return NULL;
	}

	atomic_set(&ctrl->ref_count, 1);
	ctrl->free_cb = free_cb;
	ctrl->backend = be;
	__ASSERT_NO_MSG(atomic_get(&ctrl->ref_count) > 0 && ctrl->backend != NULL);

	return mem_from_ctrl(ctrl);
}

void *memref_calloc(const struct memref_backend *be, size_t nmemb, size_t size, memref_free_cb_t free_cb)
{
	size_t payload;
	void *mem;

	if (unlikely(size_mul_overflow(nmemb, size, &payload))) {
		return NULL;
	}

	mem = memref_alloc(be, payload, free_cb);
	if (!mem) {
		return NULL;
	}
	return memset(mem, 0, payload);
}

void memref_unref(void *mem)
{
	struct ctrl_block *ctrl;
	const struct memref_backend *be;

	if (!mem) {
		return;
	}

	ctrl = ctrl_from_mem(mem);
	__ASSERT_NO_MSG(atomic_get(&ctrl->ref_count) > 0 && ctrl->backend != NULL);
	if (atomic_dec(&ctrl->ref_count) == 1) {
		be = ctrl->backend;
		if (ctrl->free_cb) {
			ctrl->free_cb(mem);
		}
		be->free(be->ctx, ctrl);
	}
}

void memref_ref(void *mem)
{
	struct ctrl_block *ctrl;
	atomic_val_t prev __maybe_unused;

	__ASSERT_NO_MSG(mem != NULL);

	ctrl = ctrl_from_mem(mem);
	__ASSERT_NO_MSG(atomic_get(&ctrl->ref_count) > 0 && ctrl->backend != NULL);
	prev = atomic_inc(&ctrl->ref_count);
	__ASSERT(0 < prev, "memref_ref called on freed memory reference");
}
