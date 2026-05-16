/*
 * Copyright (c) 2025 Måns Ansgariusson <mansgariusson@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/workq.h>
#include <zephyr/sys/min_heap.h>
#include <ksched.h>
#include <kthread.h>
#include <wait_q.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(workq, LOG_LEVEL_DBG);

struct active_work {
	uintptr_t work;
	sys_snode_t node;
};

/*FIXME: fake config_*/
#define CONFIG_WORKQ_DEFAULT_THREAD_PRIORITY 0
static struct workq_thread_config default_cfg = {
	.name = NULL,
	.prio = CONFIG_WORKQ_DEFAULT_THREAD_PRIORITY,
};

int workq_cmp(const void *a, const void *b)
{
	const struct work_item *wa = *(struct work_item *const *)a;
	const struct work_item *wb = *(struct work_item *const *)b;

	return sys_timepoint_cmp(wa->exec_time, wb->exec_time);
}

static bool work_eq(const void *node, const void *other)
{
	return *(struct work_item *const *)node == (const struct work_item *)other;
}

static inline bool workq_closed(struct workq *wq)
{
	return (wq->flags & WORKQ_FLAG_OPEN) == 0;
}

static inline bool workq_frozen(struct workq *wq)
{
	return (wq->flags & WORKQ_FLAG_FROZEN) != 0;
}

static inline bool workq_idle(struct workq *wq)
{
	return min_heap_is_empty(&wq->heap) && sys_slist_is_empty(&wq->active);
}

static inline bool active(struct workq *wq, struct work_item *work)
{
	struct active_work *a;

	SYS_SLIST_FOR_EACH_CONTAINER(&wq->active, a, node) {
		if (a->work == (uintptr_t)work) {
			return true;
		}
	}

	return false;
}

static inline struct work_item *heap_peek(struct workq *wq)
{
	struct work_item **slot = min_heap_peek(&wq->heap);

	return slot ? *slot : NULL;
}

static inline void awake(struct workq *wq)
{
	z_sched_wake(&wq->idle, 0, NULL);
}

static inline int heap_push(struct workq *wq, struct work_item *item)
{
	return min_heap_push(&wq->heap, &item);
}

static inline bool heap_remove_item(struct workq *wq, struct work_item *item)
{
	size_t id;
	struct work_item *out;

	if (min_heap_find(&wq->heap, work_eq, item, &id) == NULL) {
		return false;
	}
	return min_heap_remove(&wq->heap, id, &out);
}

static inline bool heap_contains(struct workq *wq, struct work_item *item)
{
	return min_heap_find(&wq->heap, work_eq, item, NULL) != NULL;
}

static inline int sleep_locked(struct workq *wq, k_spinlock_key_t *key, k_timeout_t timeout)
{
	int rc;

	rc = z_pend_curr(&wq->lock, *key, &wq->idle, timeout);
	*key = k_spin_lock(&wq->lock);

	return rc;
}

static inline k_timepoint_t earlier(k_timepoint_t a, k_timepoint_t b)
{
	return sys_timepoint_cmp(a, b) <= 0 ? a : b;
}

int workq_run(struct workq *wq, k_timeout_t timeout)
{
	int rc;
	work_fn_t fn;
	struct work_item *work = NULL;
	struct active_work act;
	k_timepoint_t deadline = sys_timepoint_calc(timeout);
	k_spinlock_key_t key = k_spin_lock(&wq->lock);

	for (;;) {
		struct work_item *head;
		k_timepoint_t target;
		k_timeout_t sleep_to;

		head = workq_frozen(wq) ? NULL : heap_peek(wq);
		if (head != NULL && sys_timepoint_expired(head->exec_time)) {
			struct work_item *popped;

			(void)min_heap_pop(&wq->heap, &popped);
			work = popped;
			break;
		}

		if (sys_timepoint_expired(deadline)) {
			k_spin_unlock(&wq->lock, key);
			return -EAGAIN;
		}

		target = (head != NULL) ? earlier(deadline, head->exec_time) : deadline;
		sleep_to = sys_timepoint_timeout(target);

		rc = sleep_locked(wq, &key, sleep_to);
		if (rc != 0 && rc != -EAGAIN) {
			k_spin_unlock(&wq->lock, key);
			return rc;
		}
	}

	fn = work->fn;
	act.work = (uintptr_t)work;
	sys_slist_append(&wq->active, &act.node);
	k_spin_unlock(&wq->lock, key);

	__ASSERT(fn != NULL, "Work item has not been initialized properly");
	fn(work); /* "work" should be freeable during this function call */

	K_SPINLOCK(&wq->lock) {
		sys_slist_find_and_remove(&wq->active, &act.node);
		if (workq_idle(wq)) {
			z_sched_wake_all(&wq->drain, 0, NULL);
		}
	}

	return 0;
}

void workq_init(struct workq *wq, struct work_item **storage, size_t cap)
{
	wq->flags = 0;
	wq->lock = (struct k_spinlock){};
	min_heap_init(&wq->heap, storage, cap, sizeof(struct work_item *), workq_cmp);
	sys_slist_init(&wq->active);
	z_waitq_init(&wq->idle);
	z_waitq_init(&wq->drain);
	workq_open(wq);
}

void workq_open(struct workq *wq)
{
	K_SPINLOCK(&wq->lock) {
		wq->flags |= WORKQ_FLAG_OPEN;
		awake(wq);
	}
}

void workq_close(struct workq *wq)
{
	K_SPINLOCK(&wq->lock) {
		wq->flags &= ~WORKQ_FLAG_OPEN;
	}
}

void workq_freeze(struct workq *wq)
{
	K_SPINLOCK(&wq->lock) {
		wq->flags &= ~WORKQ_FLAG_OPEN;
		wq->flags |= WORKQ_FLAG_FROZEN;
		z_sched_wake_all(&wq->idle, 0, NULL);
	}
}

void workq_thaw(struct workq *wq)
{
	K_SPINLOCK(&wq->lock) {
		if ((wq->flags & WORKQ_FLAG_FROZEN) == 0) {
			K_SPINLOCK_BREAK;
		}
		wq->flags &= ~WORKQ_FLAG_FROZEN;
		wq->flags |= WORKQ_FLAG_OPEN;
		z_sched_wake_all(&wq->idle, 0, NULL);
	}
}

void work_init(struct work_item *item, work_fn_t fn)
{
	item->fn = fn;
}

int workq_submit(struct workq *wq, struct work_item *item)
{
	int rc = 0;

	K_SPINLOCK(&wq->lock) {
		if (workq_closed(wq)) {
			rc = -EAGAIN;
			K_SPINLOCK_BREAK;
		}
		if (heap_contains(wq, item)) {
			rc = -EALREADY;
			K_SPINLOCK_BREAK;
		}
		item->exec_time = sys_timepoint_calc(K_NO_WAIT);
		if (heap_push(wq, item) != 0) {
			rc = -ENOMEM;
			K_SPINLOCK_BREAK;
		}
		awake(wq);
	}

	return rc;
}

int workq_delayed_submit(struct workq *wq, struct work_item *item, k_timeout_t delay)
{
	int rc = 0;

	K_SPINLOCK(&wq->lock) {
		if (workq_closed(wq)) {
			rc = -EBUSY;
			K_SPINLOCK_BREAK;
		}
		if (heap_contains(wq, item)) {
			rc = -EALREADY;
			K_SPINLOCK_BREAK;
		}
		item->exec_time = sys_timepoint_calc(delay);
		if (heap_push(wq, item) != 0) {
			rc = -ENOMEM;
			K_SPINLOCK_BREAK;
		}
		awake(wq);
	}

	return rc;
}

int workq_reschedule(struct workq *wq, struct work_item *item, k_timeout_t delay)
{
	int rc = 0;

	K_SPINLOCK(&wq->lock) {
		if (active(wq, item)) {
			rc = -EBUSY;
			K_SPINLOCK_BREAK;
		}
		(void)heap_remove_item(wq, item);
		item->exec_time = sys_timepoint_calc(delay);
		if (heap_push(wq, item) != 0) {
			rc = -ENOMEM;
			K_SPINLOCK_BREAK;
		}
		awake(wq);
	}

	return rc;
}

int workq_cancel(struct workq *wq, struct work_item *item)
{
	int rc = 0;

	K_SPINLOCK(&wq->lock) {
		if (active(wq, item)) {
			rc = -EBUSY;
			K_SPINLOCK_BREAK;
		}
		if (!heap_remove_item(wq, item)) {
			rc = -ENOENT;
			K_SPINLOCK_BREAK;
		}
		awake(wq);
	}

	return rc;
}

int workq_drain(struct workq *wq, k_timeout_t timeout)
{
	k_spinlock_key_t key = k_spin_lock(&wq->lock);

	if (workq_idle(wq)) {
		k_spin_unlock(&wq->lock, key);
		return 0;
	}

	return z_pend_curr(&wq->lock, key, &wq->drain, timeout);
}

static inline bool thread_running(struct workq_thread *wqt)
{
	bool running;

	K_SPINLOCK(&wqt->lock) {
		running = (wqt->flags & WORKQ_THREAD_FLAG_RUNNING) != 0;
	}

	return running;
}

void workq_thread_fn(void *arg1, void *arg2, void *arg3)
{
	int rc;
	struct workq_thread *wqt = (struct workq_thread *)arg1;

	LOG_DBG("[%p] Workq thread entered", &wqt->thread);
	while (thread_running(wqt)) {
		rc = workq_run(wqt->wq, K_FOREVER);
		if (unlikely(rc != 0 && rc != -EAGAIN)) {
			break;
		}
	}

	K_SPINLOCK(&wqt->lock) {
		wqt->flags &= ~WORKQ_THREAD_FLAG_RUNNING;
	}
	LOG_DBG("[%p] Workq thread exited", &wqt->thread);
}

void workq_thread_init(struct workq_thread *wt, struct workq *wq, k_thread_stack_t *stack,
		size_t stack_size, struct workq_thread_config *cfg)
{
	wt->wq = wq;
	wt->flags = WORKQ_THREAD_FLAG_INITIALIZED;
	wt->lock = (struct k_spinlock){};

	wt->stack = stack;
	wt->stack_size = stack_size;
	wt->cfg = cfg ? cfg : &default_cfg;
}

int workq_thread_start(struct workq_thread *wqt)
{
	int rc = 0;
	k_tid_t tid;

	K_SPINLOCK(&wqt->lock) {
		if ((wqt->flags & WORKQ_THREAD_FLAG_INITIALIZED) == 0) {
			LOG_ERR("Workq thread not initialized");
			rc = -ENODEV;
			K_SPINLOCK_BREAK;
		} else if ((wqt->flags & WORKQ_THREAD_FLAG_RUNNING) != 0) {
			LOG_ERR("Workq thread already running");
			rc = -EALREADY;
			K_SPINLOCK_BREAK;
		}
		tid = k_thread_create(&wqt->thread, wqt->stack, wqt->stack_size,
				workq_thread_fn, wqt, NULL, NULL,
				wqt->cfg->prio, 0, K_NO_WAIT);
		if (tid == NULL) {
			LOG_ERR("Failed to create workq thread");
			rc = -EINVAL;
			K_SPINLOCK_BREAK;
		}
		if (wqt->cfg->name) {
			if (k_thread_name_set(&wqt->thread, wqt->cfg->name)) {
				LOG_ERR("Failed to set workq thread name:%d", rc);
			}
		}
		wqt->flags |= WORKQ_THREAD_FLAG_RUNNING;
		LOG_DBG("[%p] Workq thread started", &wqt->thread);
	}
	return rc;
}

int workq_thread_stop(struct workq_thread *wqt, k_timeout_t timeout)
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
