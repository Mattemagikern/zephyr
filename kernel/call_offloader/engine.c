/*
 * Copyright (c) 2026 Måns Ansgariusson <mansgariusson@gmail.com>
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/kernel.h>
#include <zephyr/sys/zassert.h>
#include <zephyr/call_offloader/call_offloader.h>
#include <zephyr/internal/syscall_handler.h>

#include <ksched.h>
#include <kswap.h>
#include <kthread.h>
#include <wait_q.h>
#include <kspinlock.h>
#include <scheduler.h>
#include <kernel_internal.h>

ZASSERT_MODULE(DEFAULT);

#define CALL_OFFLOADER_DIED 0
#define CALL_OFFLOADER_DONE 1

struct call_offloader_req {
	call_offloader_work_fn_t fn;
	void *args;
};

struct pending_count {
	int prio;
	size_t count;
};

static ALWAYS_INLINE bool is_engine_thread(struct call_offloader_engine *engine,
					   struct k_thread *thread)
{
	for (size_t i = 0; i < engine->num_slots; i++) {
		if (thread == engine->workers[i].thread) {
			return true;
		}
	}

	return false;
}

static int count_pending_walk_op(struct k_thread *thread, void *data)
{
	struct pending_count *ctx = data;

	if (z_is_prio_higher_or_equal(thread->base.prio, ctx->prio)) {
		ctx->count++;
	}

	return 0;
}

static void boost_lowest_priority_slot(struct call_offloader_engine *engine, int prio)
{
	struct pending_count pending = { .prio = prio, .count = 1 };
	struct call_offloader_slot *lowest = NULL;
	int lowest_prio = prio;
	size_t have = 0;

	ZASSERT(sys_slist_is_empty(&engine->free), "boost with a free slot");

	z_sched_waitq_walk(&engine->wait_q, count_pending_walk_op, NULL, &pending);

	for (size_t i = 0; i < engine->num_slots; i++) {
		struct call_offloader_slot *slot = &engine->workers[i];

		if (z_is_prio_higher_or_equal(slot->prio, prio)) {
			have++;
		} else if (z_is_prio_higher(lowest_prio, slot->prio)) {
			lowest_prio = slot->prio;
			lowest = slot;
		}
	}

	if (lowest != NULL && have < pending.count) {
		lowest->prio = prio;
		z_thread_prio_set(lowest->thread, prio);
	}
}

static struct call_offloader_slot *acquire_slot(struct call_offloader_engine *engine, int prio)
{
	k_spinlock_key_t key = k_spin_lock(&engine->lock);
	sys_snode_t *node = sys_slist_get(&engine->free);
	struct call_offloader_slot *slot;

	if (node != NULL) {
		slot = CONTAINER_OF(node, struct call_offloader_slot, node);
		slot->prio = prio;
		k_spin_unlock(&engine->lock, key);
	} else {
		boost_lowest_priority_slot(engine, prio);
		z_pend_curr(&engine->lock, key, &engine->wait_q, K_FOREVER);
		slot = _current->base.swap_data;
	}

	if (!z_is_thread_dead(slot->thread)) {
		if (z_is_prio_higher(prio, slot->thread->base.prio)) {
			z_thread_prio_set(slot->thread, prio);
		}
		k_thread_join(slot->thread, K_FOREVER);
	}

	return slot;
}

static void release_slot(struct call_offloader_engine *engine, struct call_offloader_slot *slot, bool release_client)
{
	struct k_thread *next;
	struct k_thread *client;

	K_SPINLOCK(&engine->lock) {
		LOCK_SCHED_SPINLOCK {
			next = z_unpend_first_thread_locked(&engine->wait_q);
			if (next != NULL) {
				slot->prio = next->base.prio;
				z_thread_return_value_set_with_data(next, 0, slot);
				z_sched_ready_locked(next);
			}

			if (release_client) {
				client = z_unpend_first_thread_locked(&_current->join_queue);
				if (client != NULL) {
					z_thread_return_value_set_with_data(client,
						CALL_OFFLOADER_DONE, NULL);
					z_sched_ready_locked(client);
				}
			}
		}

		if (next == NULL) {
			sys_slist_prepend(&engine->free, &slot->node);
		}
	}
}

static FUNC_NORETURN void finish(struct call_offloader_engine *engine,
				 struct call_offloader_slot *slot)
{
	release_slot(engine, slot, true);
	k_thread_abort(_current);
	CODE_UNREACHABLE;
}

static void slot_entry(void *p1, void *p2, void *p3)
{
	struct call_offloader_engine *engine = p1;
	struct call_offloader_slot *slot = p2;
	struct call_offloader_req *req = p3;

	req->fn(req->args);

	finish(engine, slot);
}

static struct call_offloader_slot *find_current_slot(struct call_offloader_engine **engine)
{
	STRUCT_SECTION_FOREACH(call_offloader_engine, e) {
		for (size_t i = 0; i < e->num_slots; i++) {
			if (e->workers[i].thread == _current) {
				*engine = e;
				return &e->workers[i];
			}
		}
	}

	return NULL;
}

void z_impl_z_call_offloader_done(void)
{
	struct call_offloader_engine *engine = NULL;
	struct call_offloader_slot *slot = find_current_slot(&engine);

	ZASSERT(slot != NULL, "not an offloader thread");

	finish(engine, slot);
}

#ifdef CONFIG_USERSPACE
static inline void z_vrfy_z_call_offloader_done(void)
{
	struct call_offloader_engine *engine = NULL;
	struct call_offloader_slot *slot = find_current_slot(&engine);

	K_OOPS(K_SYSCALL_VERIFY_MSG(slot != NULL, "not an offloader thread"));

	finish(engine, slot);
}
#include <zephyr/syscalls/z_call_offloader_done_mrsh.c>
#endif /* CONFIG_USERSPACE */

static int start_and_join(struct k_thread *thread)
{
	k_spinlock_key_t key = z_sched_spinlock_lock();

	z_mark_thread_as_not_sleeping(thread);
	z_sched_ready_locked(thread);
	z_sched_add_to_waitq_locked(_current, &thread->join_queue);

	return z_swap_locked(key);
}

void call_offloader_dispatch(struct call_offloader_engine *engine, call_offloader_work_fn_t fn,
			     void *args)
{
	int prio;
	struct call_offloader_slot *slot;
	struct call_offloader_req req = {
		.fn = fn,
		.args = args,
	};

	ZASSERT(engine != NULL && fn != NULL);
	ZASSERT(!k_is_in_isr(), "Cannot dispatch from ISR context");
	ZASSERT(!k_is_pre_kernel(), "Cannot dispatch before the kernel has started");

	if (is_engine_thread(engine, _current)) {
		fn(args);
		return;
	}

	prio = _current->base.prio;
	slot = acquire_slot(engine, prio);

	k_thread_create(slot->thread, slot->stack, engine->stack_size, slot_entry, engine,
			      slot, &req, prio, 0, K_FOREVER);
	if (IS_ENABLED(CONFIG_THREAD_NAME)) {
		(void)k_thread_name_set(slot->thread, k_thread_name_get(_current));
	}

	if (slot->prio != prio) {
		K_SPINLOCK(&engine->lock) {
			z_thread_prio_set(slot->thread, slot->prio);
		}
	}

	if (unlikely(start_and_join(slot->thread) == CALL_OFFLOADER_DIED)) {
		release_slot(engine, slot, false);
		k_thread_abort(_current);
		CODE_UNREACHABLE;
	}
}
