/*
 * Copyright (c) 2025 Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/kernel.h>
#include <zephyr/sys/memref.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/sys/math_extras.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(memref, LOG_LEVEL_DBG);

#define MEMREF_MAGIC 0xC0FFEE00

struct ctrl_block {
#ifdef CONFIG_MEMREF_VALIDATION
	uint32_t magic;
#endif
	atomic_t ref_count;
	memref_free_cb_t free_cb;
};

void *memref_alloc(const struct memref_backend *be, size_t size, memref_free_cb_t free_cb)
{
	struct ctrl_block *ctrl;

#ifdef CONFIG_MEMREF_VALIDATION
	__ASSERT_NO_MSG(be != NULL);
#endif
	if (size_add_overflow(size, sizeof(*ctrl), &size)) {
		return NULL;
	}

	ctrl = be->alloc((void *)be->ctx, size);
	if (!ctrl) {
		return NULL;
	}

#ifdef CONFIG_MEMREF_VALIDATION
	ctrl->magic = MEMREF_MAGIC;
#endif
	atomic_set(&ctrl->ref_count, 1);
	ctrl->free_cb = free_cb;
	return ctrl + 1;
}

void *memref_calloc(const struct memref_backend *be, size_t nmemb, size_t size, memref_free_cb_t free_cb)
{
	void *mem = memref_alloc(be, nmemb * size, free_cb);
	if (!mem) {
		LOG_ERR("memref_calloc: allocation failed");
		return NULL;
	}

	memset(mem, 0, nmemb * size);
	return mem;
}

void memref_unref(const struct memref_backend *be, void *mem)
{
	struct ctrl_block *ctrl;

	if (!mem) {
		return;
	}

	ctrl = ((struct ctrl_block *)mem) - 1;
#ifdef CONFIG_MEMREF_VALIDATION
	__ASSERT_NO_MSG(ctrl->magic == MEMREF_MAGIC);
	__ASSERT_NO_MSG(atomic_get(&ctrl->ref_count) > 0);
#endif

	/* atomic_dec returns the value before decrementing, 1 => 0 refs after decrementation */
	if (atomic_dec(&ctrl->ref_count) == 1) {
		if (ctrl->free_cb) {
			ctrl->free_cb(mem);
		}
		be->free((void *)be->ctx, ctrl);
	}
}

void memref_ref(const struct memref_backend *be, void *mem)
{
	(void)be; /* for symetry */
	struct ctrl_block *ctrl;

	if (!mem) {
		return;
	}

	ctrl = (struct ctrl_block *)mem - 1;
#ifdef CONFIG_MEMREF_VALIDATION
	if (ctrl->magic != MEMREF_MAGIC) {
		LOG_ERR("memref_ref called with invalid memory reference");
	}
	__ASSERT_NO_MSG(ctrl->magic == MEMREF_MAGIC);
#endif
	if (atomic_inc(&ctrl->ref_count) == 0) {
		atomic_set(&ctrl->ref_count, 0);
		__ASSERT(false, "memref_ref called on freed memory reference");
	}
}
