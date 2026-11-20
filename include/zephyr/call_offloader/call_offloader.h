/*
 * Copyright (c) 2026 Måns Ansgariusson <mansgariusson@gmail.com>
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Generic call offloading engine.
 *
 * An offloading engine is a set of stack slots. A caller hands a function and an argument blob to
 * call_offloader_dispatch(), which starts a thread on a free slot to run it and blocks until that
 * thread has finished, so the stack depth of the call is paid once per slot instead of once by
 * every calling thread.
 */
#ifndef ZEPHYR_INCLUDE_CALL_OFFLOADER_CALL_OFFLOADER_H_
#define ZEPHYR_INCLUDE_CALL_OFFLOADER_CALL_OFFLOADER_H_

#include <zephyr/kernel.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/util.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup call_offloader Call offloading
 * @ingroup os_services
 * @{
 */
/** @cond INTERNAL_HIDDEN */
struct call_offloader_slot {
	sys_snode_t node;
	struct k_thread *thread;
	k_thread_stack_t *stack;
	int prio;
};
/** @endcond */

/** @brief An offloading engine and the stack slots serving it. */
struct call_offloader_engine {
	/** @cond INTERNAL_HIDDEN */
	struct k_spinlock lock;
	_wait_q_t wait_q;
	sys_slist_t free;
	size_t num_slots;
	size_t stack_size;
	struct call_offloader_slot *workers;
	/** @endcond */
};

/**
 * @brief Function executed by an offloading engine on behalf of a caller.
 *
 * @param args Argument blob passed to call_offloader_dispatch().
 */
typedef void (*call_offloader_work_fn_t)(void *args);

/**
 * @brief Run a function on an offloading engine and wait for it to complete.
 *
 * Takes a free slot of @p engine, starts a thread on it that runs @p fn, and blocks until that
 * thread has finished. When no slot is free the caller waits for one, in priority order. When the
 * caller is itself a thread of @p engine, @p fn is called inline instead, as waiting for a slot
 * would deadlock.
 *
 * @param engine Engine to run the function on.
 * @param fn Function to run.
 * @param args Argument blob handed to @p fn. It must stay valid for the duration of the call.
 *
 * @note The function runs at the priority of the caller.
 * @note Must not be called from an ISR or before the kernel has started.
 * @note A fatal error inside @p fn kills the thread running it. The engine then frees its slot
 *	 and aborts the caller, so this function does not return in that case.
 * @note The caller must not be aborted while this function is blocked, as the engine thread is
 *	 still reading @p args from the caller's stack.
 */
void call_offloader_dispatch(struct call_offloader_engine *engine, call_offloader_work_fn_t fn,
			     void *args);

/** @cond INTERNAL_HIDDEN */
__syscall void z_call_offloader_done(void);
/** @endcond */

/** @cond INTERNAL_HIDDEN */
#define Z_CALL_OFFLOADER_DEAD_THREAD                                         \
	{                                                                    \
		.base.thread_state = _THREAD_DEAD,                           \
		IF_ENABLED(CONFIG_USE_SWITCH, (.switch_handle = (void *)1,)) \
	}

#define Z_CALL_OFFLOADER_SLOT_OBJECTS(_i, _name, _stack_size)                \
	static K_THREAD_STACK_DEFINE(_name##_stack_##_i, _stack_size);       \
	static struct k_thread _name##_thread_##_i = Z_CALL_OFFLOADER_DEAD_THREAD

#define Z_CALL_OFFLOADER_SLOT(_i, _name, _num_slots)                         \
	{                                                                    \
		.node.next = ((_i) + 1 < (_num_slots)) ?                     \
			     &_name##_workers[(_i) + 1].node : NULL,         \
		.thread = &_name##_thread_##_i,                              \
		.stack = _name##_stack_##_i,                                 \
	}
/** @endcond */

/**
 * @brief Define an offloading engine and its stack slots.
 *
 * Defines the engine @p _name at file scope together with its slots and their stacks. No thread
 * exists until a call is dispatched, so the engine can be used from any point after the kernel
 * has started, including POST_KERNEL initialization.
 *
 * @param _name Name of the engine.
 * @param _num_slots Number of calls the engine can run concurrently. Must be an integer literal,
 *		     or a macro expanding to one, such as a Kconfig option.
 * @param _stack_size Stack size of each slot, in bytes.
 */
#define CALL_OFFLOADER_ENGINE_DEFINE(_name, _num_slots, _stack_size)                    \
	BUILD_ASSERT((_num_slots) > 0, "an offloading engine needs at least one slot"); \
	LISTIFY(_num_slots, Z_CALL_OFFLOADER_SLOT_OBJECTS, (;), _name, _stack_size);    \
	static struct call_offloader_slot _name##_workers[_num_slots] = {               \
		LISTIFY(_num_slots, Z_CALL_OFFLOADER_SLOT, (,), _name, _num_slots)      \
	};                                                                              \
	static STRUCT_SECTION_ITERABLE(call_offloader_engine, _name) = {                \
		.wait_q = Z_WAIT_Q_INIT(&_name.wait_q),                                 \
		.free = {                                                               \
			.head = &_name##_workers[0].node,                               \
			.tail = &_name##_workers[(_num_slots) - 1].node,                \
		},                                                                      \
		.num_slots = _num_slots,                                                \
		.stack_size = K_THREAD_STACK_SIZEOF(_name##_stack_0),                   \
		.workers = _name##_workers,                                             \
	}

/** @cond INTERNAL_HIDDEN */
#define Z_CALL_OFFLOADER_DROP_TYPE(...)

#define Z_CALL_OFFLOADER_PARAM(_p) __DEBRACKET _p
#define Z_CALL_OFFLOADER_FIELD(_p) __DEBRACKET _p;
#define Z_CALL_OFFLOADER_INIT(_p)                                              \
	.Z_CALL_OFFLOADER_DROP_TYPE _p = Z_CALL_OFFLOADER_DROP_TYPE _p
#define Z_CALL_OFFLOADER_ARG(_p) z_co->Z_CALL_OFFLOADER_DROP_TYPE _p

#define Z_CALL_OFFLOADER_PARAMS(...)                                           \
	COND_CODE_1(IS_EMPTY(__VA_ARGS__), (void),                             \
		    (FOR_EACH(Z_CALL_OFFLOADER_PARAM, (,), __VA_ARGS__)))
#define Z_CALL_OFFLOADER_FIELDS(...)                                           \
	COND_CODE_1(IS_EMPTY(__VA_ARGS__), (),                                 \
		    (FOR_EACH(Z_CALL_OFFLOADER_FIELD, (), __VA_ARGS__)))
#define Z_CALL_OFFLOADER_INITS(...)                                            \
	COND_CODE_1(IS_EMPTY(__VA_ARGS__), (0),                                \
		    (FOR_EACH(Z_CALL_OFFLOADER_INIT, (,), __VA_ARGS__)))
#define Z_CALL_OFFLOADER_ARGS(...)                                             \
	COND_CODE_1(IS_EMPTY(__VA_ARGS__), (),                                 \
		    (FOR_EACH(Z_CALL_OFFLOADER_ARG, (,), __VA_ARGS__)))
/** @endcond */

/**
 * @brief Define a function that runs on an offloading engine.
 *
 * Defines @c call_offloader_<_fn>() with the given signature, together with the static function
 * carrying the body that follows the macro. Callers of the wrapper block while a worker thread of
 * @p _engine runs that body, so only the worker stacks have to accommodate it.
 *
 * The body is written as an ordinary function body and refers to the parameters by name:
 *
 * @code
 * CALL_OFFLOADER_WRAP(my_engine, int, thing, (const uint8_t *) input, (size_t) input_len)
 * {
 *         return thing(input, input_len);
 * }
 * @endcode
 *
 * @param _engine Engine to run the body on.
 * @param _ret Return type of the body and of the wrapper. It must not be @c void.
 * @param _fn Name of the wrapper, without the @c call_offloader_ prefix the macro adds.
 * @param ... Parameters, each written as the type in parentheses followed by the name, as in
 *	  <tt>(const uint8_t *) input</tt>. Types whose declarator encloses the name, such as
 *	  function pointer and array types, need a typedef. At most 64 parameters.
 *
 * @note The arguments are copied onto the caller's stack for the duration of the call, so a
 *	 pointer parameter still refers to memory owned by the caller.
 */
#define CALL_OFFLOADER_WRAP(_engine, _ret, _fn, ...)                                     \
	struct z_call_offloader_##_fn##_args {                                           \
		Z_CALL_OFFLOADER_FIELDS(__VA_ARGS__)                                     \
		_ret z_co_ret;                                                           \
	};                                                                               \
	static _ret z_call_offloader_##_fn##_offloaded(                                  \
		Z_CALL_OFFLOADER_PARAMS(__VA_ARGS__));                                   \
	static void z_call_offloader_##_fn##_work(void *args)                            \
	{                                                                                \
		struct z_call_offloader_##_fn##_args *z_co = args;                       \
                                                                                         \
		z_co->z_co_ret = z_call_offloader_##_fn##_offloaded(                     \
			Z_CALL_OFFLOADER_ARGS(__VA_ARGS__));                             \
	}                                                                                \
	_ret call_offloader_##_fn(Z_CALL_OFFLOADER_PARAMS(__VA_ARGS__))                  \
	{                                                                                \
		struct z_call_offloader_##_fn##_args z_co = {                            \
			Z_CALL_OFFLOADER_INITS(__VA_ARGS__)                              \
		};                                                                       \
                                                                                         \
		call_offloader_dispatch(&_engine, z_call_offloader_##_fn##_work, &z_co); \
                                                                                         \
		return z_co.z_co_ret;                                                    \
	}                                                                                \
	static _ret z_call_offloader_##_fn##_offloaded(Z_CALL_OFFLOADER_PARAMS(__VA_ARGS__))

/**
 * @brief Define a replacement for an existing function that runs on an offloading engine.
 *
 * Same as CALL_OFFLOADER_WRAP(), for the case where @p _fn names an existing function and
 * @c call_offloader_<_fn>() is meant to replace it at its call sites. The wrapper is declared with
 * the type of @p _fn, so a parameter list or return type that does not match @p _fn is a compile
 * error instead of a silent mismatch between the callers and the wrapper.
 *
 * @code
 * CALL_OFFLOADER_WRAP_API(my_engine, int, thing, (const uint8_t *) input, (size_t) input_len)
 * {
 *         return thing(input, input_len);
 * }
 * @endcode
 *
 * @param _engine Engine to run the body on.
 * @param _ret Return type of @p _fn. It must not be @c void.
 * @param _fn Existing function the wrapper replaces.
 * @param ... Parameters of @p _fn, written as for CALL_OFFLOADER_WRAP().
 *
 * @note @p _fn must not be redirected to its wrapper where this macro is used, or the wrapper is
 *	 declared with its own type and nothing is checked.
 */
#define CALL_OFFLOADER_WRAP_API(_engine, _ret, _fn, ...)      \
	__typeof__(_fn) call_offloader_##_fn;                 \
	CALL_OFFLOADER_WRAP(_engine, _ret, _fn, __VA_ARGS__)

/** @} */


#ifdef __cplusplus
}
#endif

#include <zephyr/syscalls/call_offloader.h>

#endif /* ZEPHYR_INCLUDE_CALL_OFFLOADER_CALL_OFFLOADER_H_ */
