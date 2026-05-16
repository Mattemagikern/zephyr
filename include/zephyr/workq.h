/*
 * Copyright (c) 2025 Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _WORKQ_H
#define _WORKQ_H
#include <zephyr/kernel.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/min_heap.h>

struct work_item;
typedef void (*work_fn_t)(struct work_item *work);

enum workq_flags {
	WORKQ_FLAG_OPEN = BIT(0),
	WORKQ_FLAG_FROZEN = BIT(1),
};

enum workq_thread_flags {
	WORKQ_THREAD_FLAG_INITIALIZED = BIT(0),
	WORKQ_THREAD_FLAG_RUNNING = BIT(1),
};

struct work_item {
	/* private members */
	work_fn_t fn;
	k_timepoint_t exec_time;
};

struct workq {
	/* private members */
	uint32_t flags;
	struct k_spinlock lock;
	struct min_heap heap;
	sys_slist_t active;
	_wait_q_t idle;
	_wait_q_t drain;
};

/* should probbably be integrated into the wqthread struct instead */
struct workq_thread_config {
	const char *name;
	int prio;
};

struct workq_thread {
	/* private members */
	struct k_spinlock lock;
	struct workq *wq;
	uint32_t flags;

	struct workq_thread_config *cfg;
	struct k_thread thread;
	k_thread_stack_t *stack;
	size_t stack_size;
};

/**
 * @brief Comparator for work items in the workq min-heap.
 *
 * Compares two slots (each pointing to a `struct work_item *`) by their
 * @ref work_item::exec_time so that the earliest item is the heap root.
 *
 * @param a Pointer to the first heap slot.
 * @param b Pointer to the second heap slot.
 * @retval <0 if @p a should run before @p b
 * @retval >0 if @p a should run after @p b
 * @retval 0 if equal
 */
int workq_cmp(const void *a, const void *b);

/**
 * @brief Initialize a work item
 *
 * @param item Work item to initialize
 * @param fn Function to call when the work item is executed
 */
void work_init(struct work_item *item, work_fn_t fn);

/** @brief Initialize a work queue and set it to the open state
 *
 * @param wq Work queue to initialize
 * @param storage Backing storage for the min-heap. Must point to an array of
 *                @p cap `struct work_item *` entries.
 * @param cap Capacity of the queue (max number of pending/delayed items).
 */
void workq_init(struct workq *wq, struct work_item **storage, size_t cap);

/**
 * @brief Opens a work queue for submission
 *
 * @param wq Work queue to initialize
 */
void workq_open(struct workq *wq);

/**
 * @brief closes a work queue for submission
 *
 * @param wq Work queue to close
 */
void workq_close(struct workq *wq);

/**
 * @brief closes & cancels timers for a work queue for submission
 *
 * @param wq Work queue to stop
 */
void workq_freeze(struct workq *wq);

/**
 * @brief Opens a & resume scheduling of delayed work items
 *
 * @param wq Work queue to thaw
 */
void workq_thaw(struct workq *wq);

/**
 * @brief Run the work queue
 *
 * @param wq Work queue to run
 * @param timeout Maximum time to wait for work to be available
 * @retval 0 if work was executed, -EAGAIN if timed out
 */
int workq_run(struct workq *wq, k_timeout_t timeout);

/**
 * @brief Submit a work item to a work queue
 *
 * @param wq Work queue to submit to
 * @param item Work item to submit
 * @retval 0 if work was submitted
 * @retval -EAGAIN if the work queue is closed
 * @retval -EALREADY if the work item is already pending
 * @retval -ENOMEM if the work queue is full
 */
int workq_submit(struct workq *wq, struct work_item *item);

/**
 * @brief Submit a work item to a work queue with a delay
 *
 * @param wq Work queue to submit to
 * @param item Work item to submit
 * @param delay Delay before executing the work item
 * @retval 0 if work was submitted
 * @retval -EBUSY if the work queue is closed
 * @retval -EALREADY if the work item is already pending
 * @retval -ENOMEM if the work queue is full
 */
int workq_delayed_submit(struct workq *wq, struct work_item *item, k_timeout_t delay);

/**
 * @brief Cancel a work item
 *
 * @param wq Work queue to cancel from
 * @param item Work item to cancel
 * @retval 0 if work was canceled
 * @retval -EBUSY if the work item is currently executing
 * @retval -ENOENT if the work item is not pending
 */
int workq_cancel(struct workq *wq, struct work_item *item);

/**
 * @brief Reschedule a work item
 *
 * @param wq Work queue to reschedule from
 * @param item Work item to reschedule
 * @param delay Delay before executing the work item
 * @retval 0 if work was rescheduled, negative errno code if failed
 */
int workq_reschedule(struct workq *wq, struct work_item *item, k_timeout_t delay);

/** @brief Drain the work queue
 *
 * @param wq Work queue to drain
 * @param timeout Maximum time to wait for work to be available
 * @retval 0 if workqueue was drained, -EAGAIN if timed out
 */
int workq_drain(struct workq *wq, k_timeout_t timeout);

/**
 * @brief Statically initialize a work queue.
 *
 * @param obj Variable name of the workq being initialized.
 * @param _storage Pointer to backing storage (`struct work_item *[cap]`).
 * @param _cap Capacity (number of slots in @p _storage).
 */
#define WORKQ_INITIALIZER(obj, _storage, _cap)					\
{										\
	.flags = WORKQ_FLAG_OPEN,						\
	.lock = (struct k_spinlock){},						\
	.heap = {								\
		.storage = (_storage),						\
		.capacity = (_cap),						\
		.elem_size = sizeof(struct work_item *),			\
		.size = 0,							\
		.cmp = workq_cmp,						\
	},									\
	.active = SYS_SLIST_STATIC_INIT(&obj.active),				\
	.idle = Z_WAIT_Q_INIT(&obj.idle),					\
	.drain = Z_WAIT_Q_INIT(&obj.drain),					\
}

/**
 * @brief Statically define a work queue with backing storage.
 *
 * @param name Symbol name for the @c workq object.
 * @param cap Capacity (max number of pending/delayed items).
 */
#define WORKQ_DEFINE(name, cap)							\
	static struct work_item *name##_storage[cap];				\
	struct workq name = WORKQ_INITIALIZER(name, name##_storage, cap)

/** @brief Start a work queue thread
 *
 * @param wt Work queue thread to initialize
 * @param wq Work queue to associate with the thread
 * @param stack Stack for the work queue thread
 * @param stack_size Size of the stack
 * @param cfg Configuration for the work queue thread
 */
void workq_thread_init(struct workq_thread *wt, struct workq *wq, k_thread_stack_t *stack,
		       size_t stack_size, struct workq_thread_config *cfg);

/**
 * @brief Start a work queue thread
 * @param wqt Work queue thread to start
 * @retval 0 if work queue thread was started, negative errno code if failed
 */
int workq_thread_start(struct workq_thread *wqt);

/**
 * @brief Stop a work queue thread
 *
 * @param wqt Work queue thread to stop
 * @param timeout Time to wait for the thread to stop
 * @retval 0 if work queue thread was stopped, negative errno code if failed
 */
int workq_thread_stop(struct workq_thread *wqt, k_timeout_t timeout);

/** @brief Default work queue thread function
 *
 * @param arg1 Pointer to the work queue thread
 * @param arg2 Unused
 * @param arg3 Unused
 */
void workq_thread_fn(void *arg1, void *arg2, void *arg3);

/** @brief Statically define a work queue thread configuration
 *
 * @param cfg_name name of the work queue thread configuration variable
 * @param thread_name name of the work queue thread
 * @param priority thread priority
 */
#define WORKQ_THREAD_CONFIG(cfg_name, thread_name, priority)	\
	static struct workq_thread_config cfg_name = {		\
		.name = #thread_name,				\
		.prio = priority,				\
	}

/** @brief Statically initialize a work queue thread configuration
 *
 * @param thread_name name of the work queue thread
 * @param priority thread priority
 */
#define WORKQ_THREAD_CONFIG_INITIALIZER(thread_name, priority)	\
	{							\
		.name = #thread_name,				\
		.prio = priority,				\
	}

/** @brief Statically define and initialize a work queue thread
 *
 * @param name name of the work queue thread variable
 * @param workq work queue to associate with the thread
 * @param stack_sz size of the thread stack
 * @param priority thread priority
 */
#define WORKQ_THREAD_DEFINE(name, workq, stack_sz, priority)					\
	K_THREAD_STACK_DEFINE(stack_##name, stack_sz);						\
	WORKQ_THREAD_CONFIG(cfg_##name, name, priority);					\
	static struct workq_thread name = {							\
		.lock = (struct k_spinlock){},							\
		.flags = WORKQ_THREAD_FLAG_INITIALIZED | WORKQ_THREAD_FLAG_RUNNING,		\
		.wq = &workq,									\
		.stack = stack_##name,								\
		.stack_size = stack_sz,								\
		.cfg = &cfg_##name,								\
	};											\
	STRUCT_SECTION_ITERABLE(_static_thread_data, _k_thread_data_##name) =			\
		Z_THREAD_INITIALIZER(&name.thread,						\
				     stack_##name, stack_sz,					\
				     workq_thread_fn, &name, NULL, NULL, priority, 0, 0, name)


/** * @brief Initialize a work queue thread
 *
 * @param obj Work queue thread object to initialize
 * @param workq Work queue to associate with the thread
 * @param cfg_name Configuration for the work queue thread
 * @param sstack Stack for the work queue thread
 * @param sstack_size Size of the stack
 */
#define WORKQ_THREAD_INITIALIZE(obj, workq, cfg_name, sstack, sstack_size)	\
	{									\
		.lock = (struct k_spinlock){},					\
		.flags = WORKQ_THREAD_FLAG_INITIALIZED,				\
		.wq = workq,							\
		.stack = sstack,						\
		.stack_size = sstack_size,					\
		.cfg = cfg_name,						\
	}

#endif /* _WORKQ_H */
