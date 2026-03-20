/*
 * Copyright (c) 2026 Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/**
 * @file
 * @brief Public API for the priority work queue variant
 * @ingroup workq_prio_apis
 */

#ifndef ZEPHYR_INCLUDE_WORKQS_PRIORITY_H_
#define ZEPHYR_INCLUDE_WORKQS_PRIORITY_H_
#include <zephyr/workqs/engine.h>

/**
 * @defgroup workq_prio_apis Priority work queue
 * @ingroup workq_apis
 * @brief Public API for the priority work queue variant
 *
 * Work items submitted to a priority work queue carry a priority and are
 * executed in ascending priority order; a lower value runs first. Items of
 * equal priority are executed in submission order.
 * @{
 */

/**
 * @brief A priority work item.
 *
 * Ready items are executed in ascending priority order (a lower @ref prio value
 * runs first). Items of equal priority run in submission order.
 */
struct workq_prio_work {
	struct work_base base;
	uint8_t prio;
};

/**
 * @brief A priority work queue.
 *
 * Embeds the shared engine with the priority ordering policy.
 */
struct workq_prio {
	struct workq_engine engine;
};

/**
 * @cond INTERNAL_HIDDEN
 */
void workq_prio_enqueue(struct workq_engine *wq, struct work_base *work);
/**
 * INTERNAL_HIDDEN @endcond
 */

/**
 * @brief Initialize a priority work item
 *
 * @param work Work item to initialize
 * @param fn Function to call when the work item is executed
 */
static inline void workq_prio_work_init(struct workq_prio_work *work, work_fn_t fn)
{
	z_work_init(&work->base, fn);
	work->prio = 0;
}

/**
 * @brief Initialize a priority work queue and set it to the open state
 *
 * @param wq Work queue to initialize
 */
static inline void workq_prio_init(struct workq_prio *wq)
{
	z_workq_engine_init(&wq->engine, workq_prio_enqueue);
}

/**
 * @brief Open a priority work queue for submission
 *
 * @param wq Work queue to open
 */
static inline void workq_prio_open(struct workq_prio *wq)
{
	z_workq_open(&wq->engine);
}

/**
 * @brief Close a priority work queue for submission
 *
 * @param wq Work queue to close
 */
static inline void workq_prio_close(struct workq_prio *wq)
{
	z_workq_close(&wq->engine);
}

/**
 * @brief Freeze scheduling of delayed work items
 *
 * @param wq Work queue to freeze
 */
static inline void workq_prio_freeze(struct workq_prio *wq)
{
	z_workq_freeze(&wq->engine);
}

/**
 * @brief Resume scheduling of delayed work items
 *
 * @param wq Work queue to thaw
 */
static inline void workq_prio_thaw(struct workq_prio *wq)
{
	z_workq_thaw(&wq->engine);
}

/**
 * @brief Run the priority work queue
 *
 * @param wq Work queue to run
 * @param timeout Maximum time to wait for work to be available
 * @retval 0 if work was executed, -EAGAIN if timed out
 */
static inline int workq_prio_run(struct workq_prio *wq, k_timeout_t timeout)
{
	return z_workq_run(&wq->engine, timeout);
}

/**
 * @brief Submit a work item to a priority work queue
 *
 * @param wq Work queue to submit to
 * @param work Work item to submit
 * @param prio Item priority; a lower value is a higher priority
 * @retval 0 if work was submitted, negative errno code if failed
 */
int workq_prio_submit(struct workq_prio *wq, struct workq_prio_work *work, uint8_t prio);

/**
 * @brief Submit a work item to a priority work queue with a delay
 *
 * @param wq Work queue to submit to
 * @param work Work item to submit
 * @param prio Item priority; a lower value is a higher priority
 * @param delay Delay before executing the work item
 * @retval 0 if work was submitted, negative errno code if failed
 */
int workq_prio_delayed_submit(struct workq_prio *wq, struct workq_prio_work *work, uint8_t prio,
			      k_timeout_t delay);

/**
 * @brief Reschedule a work item
 *
 * @param wq Work queue to reschedule from
 * @param work Work item to reschedule
 * @param prio Item priority; a lower value is a higher priority
 * @param delay Delay before executing the work item
 * @retval 0 if work was rescheduled, negative errno code if failed
 */
int workq_prio_reschedule(struct workq_prio *wq, struct workq_prio_work *work, uint8_t prio,
			  k_timeout_t delay);

/**
 * @brief Cancel a work item
 *
 * @param wq Work queue to cancel from
 * @param work Work item to cancel
 * @retval 0 if work was canceled, negative errno code if failed
 */
static inline int workq_prio_cancel(struct workq_prio *wq, struct workq_prio_work *work)
{
	return z_workq_cancel(&wq->engine, &work->base);
}

/**
 * @brief Drain the priority work queue
 *
 * @param wq Work queue to drain
 * @param timeout Maximum time to wait for the queue to drain
 * @retval 0 if the queue was drained, -EAGAIN if timed out
 */
static inline int workq_prio_drain(struct workq_prio *wq, k_timeout_t timeout)
{
	return z_workq_drain(&wq->engine, timeout);
}

/**
 * @brief Statically initialize a priority work queue in the open state.
 *
 * @param obj Name of the @ref workq_prio object being initialized.
 */
#define WORKQ_PRIO_INITIALIZER(obj)					\
{									\
	.engine = WORKQ_ENGINE_INITIALIZER(obj, workq_prio_enqueue),	\
}

/**
 * @brief Statically define and initialize a priority work queue.
 *
 * @param name Name of the @ref workq_prio object.
 */
#define WORKQ_PRIO_DEFINE(name) \
	struct workq_prio name = WORKQ_PRIO_INITIALIZER(name)

/**
 * @brief Initialize a priority work queue thread
 *
 * @param wt Work queue thread to initialize
 * @param wq Work queue the thread will service
 * @param stack Stack for the work queue thread
 * @param stack_size Size of the stack
 * @param cfg Configuration applied at start, or NULL for defaults. When
 *            non-NULL it must outlive the thread.
 */
static inline void workq_prio_thread_init(struct workq_thread *wt, struct workq_prio *wq,
					  k_thread_stack_t *stack, size_t stack_size,
					  const struct workq_thread_config *cfg)
{
	z_workq_thread_init(wt, &wq->engine, stack, stack_size, cfg);
}

/**
 * @brief Start a priority work queue thread
 *
 * @param wt Work queue thread to start
 * @retval 0 if the thread was started, negative errno code if failed
 */
static inline int workq_prio_thread_start(struct workq_thread *wt)
{
	return z_workq_thread_start(wt);
}

/**
 * @brief Stop a priority work queue thread
 *
 * @param wt Work queue thread to stop
 * @param timeout Maximum time to wait for the thread to stop
 * @retval 0 if the thread was stopped, negative errno code if failed
 */
static inline int workq_prio_thread_stop(struct workq_thread *wt, k_timeout_t timeout)
{
	return z_workq_thread_stop(wt, timeout);
}

/**
 * @brief Statically define and initialize a priority work queue thread
 *
 * @param name name of the work queue thread variable
 * @param workq @ref workq_prio queue to associate with the thread
 * @param stack_sz size of the thread stack
 * @param priority thread priority
 */
#define WORKQ_PRIO_THREAD_DEFINE(name, workq, stack_sz, priority) \
	Z_WORKQ_THREAD_DEFINE(name, &(workq).engine, stack_sz, priority)

/**
 * @brief Statically initialize a priority work queue thread
 *
 * @param obj Work queue thread object to initialize
 * @param workq @ref workq_prio queue to associate with the thread
 * @param cfg_name Configuration for the work queue thread
 * @param sstack Stack for the work queue thread
 * @param sstack_size Size of the stack
 */
#define WORKQ_PRIO_THREAD_INITIALIZE(obj, workq, cfg_name, sstack, sstack_size) \
	Z_WORKQ_THREAD_INITIALIZE(obj, &(workq)->engine, cfg_name, sstack, sstack_size)

/** @} */

#endif /* ZEPHYR_INCLUDE_WORKQS_PRIORITY_H_ */
