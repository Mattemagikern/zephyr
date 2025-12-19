/*
 * Copyright (c) 2025 Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MEMREF_H
#define MEMREF_H

/**
 * @file
 *
 * @brief Reference counted memory allocation API.
 */

typedef void *(*memref_alloc_func_t)(void *be, size_t size);
typedef void (*memref_free_func_t)(void *be, void *mem);
struct memref_backend {
	memref_alloc_func_t alloc;
	memref_free_func_t free;
	void *ctx;
};

/**
 * @defgroup _memref Reference counted memory allocation
 * @ingroup memory_management
 * @{
 */

#define MEMREF_BACKEND(name, alloc_func, free_func, backend_context)	\
	static const struct memref_backend name = {			\
		.alloc = alloc_func,					\
		.free = free_func,					\
		.ctx = backend_context,					\
	}


typedef void (*memref_free_cb_t)(void *mem);

/** Allocate memory with reference counting.
 *
 * @param size Size of the memory to allocate.
 * @param free_cb Callback function to free the memory when reference count reaches zero.
 * @return Pointer to the allocated memory, or NULL on failure.
 */
void *memref_alloc(const struct memref_backend *backend, size_t size, memref_free_cb_t free_cb);

/** Allocate zero-initialized memory with reference counting.
 *
 * @param nmemb Number of elements.
 * @param size Size of each element.
 * @param free_cb Callback function to free the memory when reference count reaches zero.
 * @return Pointer to the allocated memory, or NULL on failure.
 */
void *memref_calloc(const struct memref_backend *backend, size_t nmemb, size_t size, memref_free_cb_t free_cb);

/** Decrease the reference count of the memory.
 *
 * If the reference count reaches zero the memory is freed.
 * If the memory is to be freed the registered callback is invoked.
 *
 * @param mem Pointer to the memory to free.
 */
void memref_unref(const struct memref_backend *backend, void *mem);

/** Increase the reference count of the memory.
 *
 * @param mem Pointer to take ownership of.
 */
void memref_ref(const struct memref_backend *backend, void *mem);

/**
 * @}
 */

#endif /* MEMREF_H */
