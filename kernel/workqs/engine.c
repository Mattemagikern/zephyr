/*
 * Copyright (c) 2026 Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/workqs/engine.h>
#include <ksched.h>
#include <kthread.h>
#include <wait_q.h>
#include "internal.h"

/* private struct, only used locally */
struct active_work {
	uintptr_t work;
	sys_snode_t node;
};

#define WORKQ_DEFAULT_THREAD_PRIORITY K_LOWEST_APPLICATION_THREAD_PRIO
static const struct workq_thread_config default_cfg = {
	.name = NULL,
	.prio = WORKQ_DEFAULT_THREAD_PRIORITY,
};

/* should only be called with lock held */
static bool z_workq_closed(struct workq_engine *wq)
{
	return (wq->flags & WORKQ_FLAG_OPEN) == 0;
}

/* should only be called with lock held */
static inline bool workq_frozen(struct workq_engine *wq)
{
	return (wq->flags & WORKQ_FLAG_FROZEN) != 0;
}

/* should only be called with lock held */
static inline bool workq_idle(struct workq_engine *wq)
{
	return sys_slist_is_empty(&wq->pending) && sys_slist_is_empty(&wq->delayed) &&
		sys_slist_is_empty(&wq->active);
}

/* should only be called with lock held */
static bool z_workq_pending(struct workq_engine *wq, struct work_base *item)
{
	struct work_base *wi;

	SYS_SLIST_FOR_EACH_CONTAINER(&wq->pending, wi, node) {
		if (wi == item) {
			return true;
		}
	}

	SYS_SLIST_FOR_EACH_CONTAINER(&wq->delayed, wi, node) {
		if (wi == item) {
			return true;
		}
	}

	return false;
}

/* should only be called with lock held */
static inline bool active(struct workq_engine *wq, struct work_base *work)
{
	struct active_work *active;

	SYS_SLIST_FOR_EACH_CONTAINER(&wq->active, active, node) {
		if (active->work == (uintptr_t)work) {
			return true;
		}
	}

	return false;
}

/* should only be called with lock held */
static inline struct work_base *get_next_work(struct workq_engine *wq)
{
	sys_snode_t *node = sys_slist_get(&wq->pending);

	if (node == NULL) {
		return NULL;
	}

	return CONTAINER_OF(node, struct work_base, node);
}

/* should only be called with lock held */
void z_workq_awake(struct workq_engine *wq)
{
	z_sched_wake(&wq->idle, 0, NULL);
}

static void schedule_cb(struct _timeout *t);
/* should only be called with lock held */
static void schedule_next_timeout(struct workq_engine *wq)
{
	struct work_base *item;

	if (workq_frozen(wq)) {
		return;
	}

	if (sys_slist_is_empty(&wq->delayed)) {
		return;
	}

	item = CONTAINER_OF(sys_slist_peek_head(&wq->delayed), struct work_base, node);
	z_add_timeout(&wq->timeout, schedule_cb, sys_timepoint_timeout(item->exec_time));
}

/* should only be called with lock held */
static int z_workq_cancel_locked(struct workq_engine *wq, struct work_base *item)
{
	if (active(wq, item)) {
		/* cannot cancel running item */
		return -EBUSY;
	}

	if (sys_slist_peek_head(&wq->delayed) == &item->node) {
		(void)z_try_abort_timeout(&wq->timeout);
		sys_slist_find_and_remove(&wq->delayed, &item->node);
		schedule_next_timeout(wq);
	} else {
		sys_slist_find_and_remove(&wq->delayed, &item->node);
		sys_slist_find_and_remove(&wq->pending, &item->node);
	}

	return 0;
}

/* should only be called with lock held */
int z_workq_submit_guard(struct workq_engine *wq, struct work_base *item)
{
	if (z_workq_closed(wq)) {
		return -EAGAIN;
	}

	if (z_workq_pending(wq, item)) {
		return -EALREADY;
	}

	return 0;
}

/* should only be called with lock held */
int z_workq_reschedule_guard(struct workq_engine *wq, struct work_base *item)
{
	if (z_workq_closed(wq)) {
		return -EAGAIN;
	}

	if (z_workq_pending(wq, item)) {
		return z_workq_cancel_locked(wq, item);
	}

	return 0;
}

/* should only be called with lock held */
void z_workq_delayed_insert(struct workq_engine *wq, struct work_base *item,
			    k_timepoint_t exec_time)
{
	struct work_base *next;
	sys_snode_t *node, *prev = NULL;

	item->exec_time = exec_time;
	SYS_SLIST_FOR_EACH_NODE(&wq->delayed, node) {
		next = CONTAINER_OF(node, struct work_base, node);
		if (sys_timepoint_cmp(item->exec_time, next->exec_time) < 0) {
			sys_slist_insert(&wq->delayed, prev, &item->node);
			break;
		}
		prev = node;
	}

	if (node == NULL) {
		sys_slist_append(&wq->delayed, &item->node);
	}

	if (sys_slist_peek_head(&wq->delayed) == &item->node) {
		(void)z_try_abort_timeout(&wq->timeout);
		schedule_next_timeout(wq);
	}
}

static inline bool thread_running(struct workq_thread *wqt)
{
	bool running;

	K_SPINLOCK(&wqt->lock) {
		running = (wqt->flags & WORKQ_THREAD_FLAG_RUNNING) != 0;
	}

	return running;
}

static void schedule_cb(struct _timeout *t)
{
	struct workq_engine *wq = CONTAINER_OF(t, struct workq_engine, timeout);
	struct work_base *item;

	K_SPINLOCK(&wq->lock) {
		while (!sys_slist_is_empty(&wq->delayed)) {
			item = CONTAINER_OF(sys_slist_peek_head(&wq->delayed),
					struct work_base, node);
			if (!sys_timepoint_expired(item->exec_time)) {
				break;
			}
			sys_slist_get(&wq->delayed);
			wq->enqueue(wq, item);
			z_workq_awake(wq);
		}
		schedule_next_timeout(wq);
	}
}

static inline int sleep(struct workq_engine *wq, k_spinlock_key_t *key, k_timeout_t timeout)
{
	int rc = 0;

	rc = z_pend_curr(&wq->lock, *key, &wq->idle, timeout);
	*key = k_spin_lock(&wq->lock);

	return rc;
}

int z_workq_run(struct workq_engine *wq, k_timeout_t timeout)
{
	int rc;
	work_fn_t fn;
	struct work_base *work;
	struct active_work active;
	k_spinlock_key_t key = k_spin_lock(&wq->lock);

	while ((work = get_next_work(wq)) == NULL) {
		rc = sleep(wq, &key, timeout);
		if (rc != 0) {
			k_spin_unlock(&wq->lock, key);
			return rc;
		} else if (workq_idle(wq)) {
			while (z_sched_wake(&wq->drain, 0, NULL)) {
			}
		}
	}

	fn = work->fn;
	active.work = (uintptr_t)work;
	sys_slist_append(&wq->active, &active.node);
	k_spin_unlock(&wq->lock, key);

	__ASSERT(fn != NULL, "Work item has not been initialized properly");
	fn(work); /* "work" should be freeable during this function call */

	K_SPINLOCK(&wq->lock) {
		sys_slist_find_and_remove(&wq->active, &active.node);
		if (workq_idle(wq)) {
			while (z_sched_wake(&wq->drain, 0, NULL)) {
			}
		}
	}

	return 0;
}

void z_workq_engine_init(struct workq_engine *wq,
			 void (*enqueue)(struct workq_engine *, struct work_base *))
{
	wq->flags = 0;
	wq->lock = (struct k_spinlock){};
	wq->enqueue = enqueue;
	sys_slist_init(&wq->active);
	sys_slist_init(&wq->pending);
	sys_slist_init(&wq->delayed);
	z_init_timeout(&wq->timeout);
	z_waitq_init(&wq->idle);
	z_waitq_init(&wq->drain);
	z_workq_open(wq);
}

void z_workq_open(struct workq_engine *wq)
{
	K_SPINLOCK(&wq->lock) {
		wq->flags |= WORKQ_FLAG_OPEN;
		z_workq_awake(wq);
	}
}

void z_workq_close(struct workq_engine *wq)
{
	K_SPINLOCK(&wq->lock) {
		wq->flags &= ~WORKQ_FLAG_OPEN;
	}
}

void z_workq_freeze(struct workq_engine *wq)
{
	K_SPINLOCK(&wq->lock) {
		wq->flags |= WORKQ_FLAG_FROZEN;
		(void)z_try_abort_timeout(&wq->timeout);
	}
}

void z_workq_thaw(struct workq_engine *wq)
{
	K_SPINLOCK(&wq->lock) {
		if ((wq->flags & WORKQ_FLAG_FROZEN) == 0) {
			K_SPINLOCK_BREAK;
		}
		wq->flags &= ~WORKQ_FLAG_FROZEN;
		schedule_next_timeout(wq);
	}
}

void z_work_init(struct work_base *item, work_fn_t fn)
{
	item->fn = fn;
}

int z_workq_cancel(struct workq_engine *wq, struct work_base *item)
{
	int rc;

	K_SPINLOCK(&wq->lock) {
		if (active(wq, item)) {
			rc = -EBUSY;
			K_SPINLOCK_BREAK;
		} else if (!z_workq_pending(wq, item)) {
			rc = -ENOENT;
			K_SPINLOCK_BREAK;
		}
		rc = z_workq_cancel_locked(wq, item);
	}

	return rc;
}

int z_workq_drain(struct workq_engine *wq, k_timeout_t timeout)
{
	k_spinlock_key_t key = k_spin_lock(&wq->lock);

	if (workq_idle(wq)) {
		k_spin_unlock(&wq->lock, key);
		return 0;
	}

	return z_pend_curr(&wq->lock, key, &wq->drain, timeout);
}

void z_workq_thread_fn(void *arg1, void *arg2, void *arg3)
{
	int rc;
	struct workq_thread *wqt = (struct workq_thread *)arg1;

	while (thread_running(wqt)) {
		rc = z_workq_run(wqt->wq, K_FOREVER);
		if (unlikely(rc != 0)) {
			break;
		}
	}

	K_SPINLOCK(&wqt->lock) {
		wqt->flags &= ~WORKQ_THREAD_FLAG_RUNNING;
	}
}

void z_workq_thread_init(struct workq_thread *wt, struct workq_engine *wq, k_thread_stack_t *stack,
		size_t stack_size, const struct workq_thread_config *cfg)
{
	wt->wq = wq;
	wt->flags = WORKQ_THREAD_FLAG_INITIALIZED;
	wt->lock = (struct k_spinlock){};

	wt->stack = stack;
	wt->stack_size = stack_size;
	wt->cfg = cfg ? cfg : &default_cfg;
}

int z_workq_thread_start(struct workq_thread *wqt)
{
	int rc = 0;
	k_tid_t tid;

	K_SPINLOCK(&wqt->lock) {
		if ((wqt->flags & WORKQ_THREAD_FLAG_INITIALIZED) == 0) {
			rc = -ENODEV;
			K_SPINLOCK_BREAK;
		} else if ((wqt->flags & WORKQ_THREAD_FLAG_RUNNING) != 0) {
			rc = -EALREADY;
			K_SPINLOCK_BREAK;
		}
		tid = k_thread_create(&wqt->thread, wqt->stack, wqt->stack_size,
				z_workq_thread_fn, wqt, NULL, NULL,
				wqt->cfg->prio, 0, K_NO_WAIT);
		if (tid == NULL) {
			rc = -EINVAL;
			K_SPINLOCK_BREAK;
		}
		if (wqt->cfg->name) {
			(void)k_thread_name_set(&wqt->thread, wqt->cfg->name);
		}
		wqt->flags |= WORKQ_THREAD_FLAG_RUNNING;
	}
	return rc;
}

int z_workq_thread_stop(struct workq_thread *wqt, k_timeout_t timeout)
{
	k_spinlock_key_t key = k_spin_lock(&wqt->lock);
	k_spinlock_key_t key_q = k_spin_lock(&wqt->wq->lock);

	wqt->flags &= ~WORKQ_THREAD_FLAG_RUNNING;
	if (z_is_thread_pending(&wqt->thread)) {
		arch_thread_return_value_set(&wqt->thread, -ESHUTDOWN);
		z_unpend_thread(&wqt->thread);
		z_ready_thread(&wqt->thread);
	}
	k_spin_unlock(&wqt->wq->lock, key_q);
	k_spin_unlock(&wqt->lock, key);

	return k_thread_join(&wqt->thread, timeout);
}
