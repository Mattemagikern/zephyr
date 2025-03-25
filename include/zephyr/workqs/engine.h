/*
 * Copyright (c) 2026 Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/**
 * @file
 * @brief Shared engine for the queue-centric work queue variants
 * @ingroup workq_apis
 */

#ifndef ZEPHYR_INCLUDE_WORKQS_ENGINE_H_
#define ZEPHYR_INCLUDE_WORKQS_ENGINE_H_
#include <zephyr/kernel.h>
#include <zephyr/sys/slist.h>

/**
 * @defgroup workq_apis Work queue (workq)
 * @ingroup datastructure_apis
 * @brief Queue-centric work queue with multi-threaded execution
 *
 * The work queue is built from a shared engine and thin variants. The engine
 * owns the worker threads, the delayed-work timeout, and the queue state
 * machine; a variant (for example @ref workq_fifo or @ref workq_prio) embeds
 * the engine and supplies the ordering policy used to enqueue ready work.
 * @{
 */

struct work_base;

/**
 * @brief Signature for a work item handler function.
 *
 * @param work Work item being executed.
 */
typedef void (*work_fn_t)(struct work_base *work);

/** @brief Work queue state flags. */
enum workq_flags {
	/** Queue accepts new submissions. */
	WORKQ_FLAG_OPEN = BIT(0),
	/** Scheduling of delayed items is suspended. */
	WORKQ_FLAG_FROZEN = BIT(1),
};

/** @brief Work queue thread state flags. */
enum workq_thread_flags {
	/** Thread has been initialized and is ready to start. */
	WORKQ_THREAD_FLAG_INITIALIZED = BIT(0),
	/** Thread is currently running. */
	WORKQ_THREAD_FLAG_RUNNING = BIT(1),
};

/**
 * @brief Base work item shared by all work queue variants.
 *
 * A variant embeds this as its first member and recovers its concrete type with
 * @ref CONTAINER_OF. All the members are internal and should not be accessed
 * directly.
 */
struct work_base {
/**
 * @cond INTERNAL_HIDDEN
 */
	work_fn_t fn;
	sys_snode_t node;
	k_timepoint_t exec_time;
/**
 * INTERNAL_HIDDEN @endcond
 */
};

/**
 * @brief Shared engine backing every work queue variant.
 *
 * A variant embeds this as its first member. All the members are internal and
 * should not be accessed directly.
 */
struct workq_engine {
/**
 * @cond INTERNAL_HIDDEN
 */
	uint32_t flags;
	struct k_spinlock lock;
	struct _timeout timeout;
	sys_slist_t active;
	sys_slist_t pending;
	sys_slist_t delayed;
	_wait_q_t idle;
	_wait_q_t drain;
	void (*enqueue)(struct workq_engine *wq, struct work_base *work);
/**
 * INTERNAL_HIDDEN @endcond
 */
};

/** @brief Configuration for a work queue thread. */
struct workq_thread_config {
	/** Thread name, or NULL for an unnamed thread. */
	const char *name;
	/** Thread scheduling priority. */
	int prio;
};

/**
 * @brief A thread that services a work queue.
 *
 * All the members are internal and should not be accessed directly.
 */
struct workq_thread {
/**
 * @cond INTERNAL_HIDDEN
 */
	struct k_spinlock lock;
	struct workq_engine *wq;
	uint32_t flags;

	const struct workq_thread_config *cfg;
	struct k_thread thread;
	k_thread_stack_t *stack;
	size_t stack_size;
/**
 * INTERNAL_HIDDEN @endcond
 */
};

/**
 * @cond INTERNAL_HIDDEN
 *
 * The following declarations form the private engine contract shared by the
 * work queue variants. Applications use the typed variant wrappers instead.
 */

void z_work_init(struct work_base *work, work_fn_t fn);

void z_workq_engine_init(struct workq_engine *wq,
			 void (*enqueue)(struct workq_engine *, struct work_base *));

void z_workq_open(struct workq_engine *wq);
void z_workq_close(struct workq_engine *wq);
void z_workq_freeze(struct workq_engine *wq);
void z_workq_thaw(struct workq_engine *wq);

int z_workq_run(struct workq_engine *wq, k_timeout_t timeout);
int z_workq_drain(struct workq_engine *wq, k_timeout_t timeout);

int z_workq_cancel(struct workq_engine *wq, struct work_base *work);

void z_workq_thread_init(struct workq_thread *wt, struct workq_engine *wq, k_thread_stack_t *stack,
			 size_t stack_size, const struct workq_thread_config *cfg);
int z_workq_thread_start(struct workq_thread *wt);
int z_workq_thread_stop(struct workq_thread *wt, k_timeout_t timeout);
void z_workq_thread_fn(void *arg1, void *arg2, void *arg3);

/**
 * INTERNAL_HIDDEN @endcond
 */

/**
 * @brief Statically initialize a work queue engine in the open state.
 *
 * @param obj Name of the variant object embedding the engine.
 * @param enqueue_fn Ordering policy used to enqueue ready work.
 */
#define WORKQ_ENGINE_INITIALIZER(obj, enqueue_fn)			\
{									\
	.flags = WORKQ_FLAG_OPEN,					\
	.lock = (struct k_spinlock){},					\
	.idle = Z_WAIT_Q_INIT(&(obj).engine.idle),			\
	.drain = Z_WAIT_Q_INIT(&(obj).engine.drain),			\
	.active = SYS_SLIST_STATIC_INIT(&(obj).engine.active),		\
	.pending = SYS_SLIST_STATIC_INIT(&(obj).engine.pending),	\
	.delayed = SYS_SLIST_STATIC_INIT(&(obj).engine.delayed),	\
	.enqueue = (enqueue_fn),					\
}

/**
 * @cond INTERNAL_HIDDEN
 */

#define Z_WORKQ_THREAD_CONFIG(cfg_name, thread_name, priority)	\
	static const struct workq_thread_config cfg_name = {	\
		.name = #thread_name,				\
		.prio = priority,				\
	}

#define Z_WORKQ_THREAD_DEFINE(name, engine_ptr, stack_sz, priority)				\
	K_THREAD_STACK_DEFINE(stack_##name, stack_sz);						\
	Z_WORKQ_THREAD_CONFIG(cfg_##name, name, priority);					\
	static struct workq_thread name = {							\
		.lock = (struct k_spinlock){},							\
		.flags = WORKQ_THREAD_FLAG_INITIALIZED | WORKQ_THREAD_FLAG_RUNNING,		\
		.wq = (engine_ptr),								\
		.stack = stack_##name,								\
		.stack_size = stack_sz,								\
		.cfg = &cfg_##name,								\
	};											\
	STRUCT_SECTION_ITERABLE(_static_thread_data, _k_thread_data_##name) =			\
		Z_THREAD_INITIALIZER(&name.thread,						\
				     stack_##name, stack_sz,					\
				     z_workq_thread_fn, &name, NULL, NULL, priority, 0, 0, name)

#define Z_WORKQ_THREAD_INITIALIZE(obj, engine_ptr, cfg_name, sstack, sstack_size)	\
	{										\
		.lock = (struct k_spinlock){},						\
		.flags = WORKQ_THREAD_FLAG_INITIALIZED,					\
		.wq = (engine_ptr),							\
		.stack = sstack,							\
		.stack_size = sstack_size,						\
		.cfg = cfg_name,							\
	}

/**
 * INTERNAL_HIDDEN @endcond
 */

/**
 * @brief Statically define a work queue thread configuration
 *
 * @param cfg_name name of the work queue thread configuration variable
 * @param thread_name name of the work queue thread
 * @param priority thread priority
 */
#define WORKQ_THREAD_CONFIG(cfg_name, thread_name, priority)	\
	Z_WORKQ_THREAD_CONFIG(cfg_name, thread_name, priority)

/**
 * @brief Statically initialize a work queue thread configuration
 *
 * @param thread_name name of the work queue thread
 * @param priority thread priority
 */
#define WORKQ_THREAD_CONFIG_INITIALIZER(thread_name, priority)	\
	{							\
		.name = #thread_name,				\
		.prio = priority,				\
	}

/** @} */

#endif /* ZEPHYR_INCLUDE_WORKQS_ENGINE_H_ */
