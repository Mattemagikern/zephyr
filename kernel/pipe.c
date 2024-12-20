#include <zephyr/kernel.h>
#include <ksched.h>
#include <kthread.h>
#include <wait_q.h>
#include <errno.h>

typedef bool (*wait_cond_t)(struct k_pipe *pipe);
static inline size_t utilization(struct k_pipe *pipe)
{
	return pipe->flags & PIPE_FLAG_FULL ?
		pipe->buffer_size :
		(pipe->tail - pipe->head + pipe->buffer_size) % pipe->buffer_size;
}

static bool queue_full(struct k_pipe *pipe)
{
	return utilization(pipe) == pipe->buffer_size;
}

static bool queue_empty(struct k_pipe *pipe)
{
	return utilization(pipe) == 0;
}

static int wait_for(_wait_q_t *waitq, struct k_pipe *pipe, k_spinlock_key_t *key,
		wait_cond_t cond, k_timeout_t timeout)
{
	if (K_TIMEOUT_EQ(timeout, K_NO_WAIT) || pipe->flags & PIPE_FLAG_RESET) {
		k_spin_unlock(&pipe->lock, *key);
		return -EAGAIN;
	}
	pipe->waiting++;
	z_pend_curr(&pipe->lock, *key, waitq, timeout);
	*key = k_spin_lock(&pipe->lock);
	pipe->waiting--;
	if (unlikely(!(pipe->flags & PIPE_FLAG_OPEN))) {
		return -EPIPE;
	} else if (unlikely(pipe->flags & PIPE_FLAG_RESET)) {
		if (!pipe->waiting) {
			pipe->flags &= ~PIPE_FLAG_RESET;
		}
		return -ECANCELED;
	} else if (!cond(pipe)) {
		return 0;
	}
	return -EAGAIN;
}

static void notify_waiter(_wait_q_t *waitq)
{
	struct k_thread *thread_to_unblock = z_unpend_first_thread(waitq);
	if (likely(thread_to_unblock)) {
		z_ready_thread(thread_to_unblock);
	}
}

void z_impl_k_pipe_init(struct k_pipe *pipe, uint8_t *buffer, size_t buffer_size)
{
	pipe->buffer = buffer;
	pipe->buffer_size = buffer_size;
	pipe->head = 0U;
	pipe->tail = 0U;
	pipe->waiting = 0U;
	pipe->flags = PIPE_FLAG_OPEN;

	pipe->lock = (struct k_spinlock){0};
	z_waitq_init(&pipe->data);
	z_waitq_init(&pipe->space);
}

int z_impl_k_pipe_write(struct k_pipe *pipe, const uint8_t *data, size_t len, k_timeout_t timeout)
{
	int rc;
	size_t write_len, first_chunk, second_chunk;

	__ASSERT_NO_MSG(pipe != NULL);
	__ASSERT_NO_MSG(data != NULL);

	k_spinlock_key_t key = k_spin_lock(&pipe->lock);
	if (unlikely(queue_full(pipe))) {
		rc = wait_for(&pipe->space, pipe, &key, queue_full, timeout);
		if (rc) {
			k_spin_unlock(&pipe->lock, key);
			return rc;
		}
	}

	if (!(pipe->flags & PIPE_FLAG_OPEN)) {
		k_spin_unlock(&pipe->lock, key);
		return -EPIPE;
	}
	write_len = MIN(pipe->buffer_size - utilization(pipe), len);
	first_chunk = MIN(pipe->buffer_size - pipe->tail, write_len);
	second_chunk = write_len - first_chunk;

	memcpy(&pipe->buffer[pipe->tail], data, first_chunk);
	if (second_chunk > 0) {
		memcpy(pipe->buffer, &data[first_chunk], second_chunk);
	}

	pipe->tail = (pipe->tail + write_len) % pipe->buffer_size;


	if (write_len) {
		notify_waiter(&pipe->data);
		if(pipe->tail == pipe->head) {
			pipe->flags |= PIPE_FLAG_FULL;
		}
	}
	k_spin_unlock(&pipe->lock, key);
	return write_len;
}

int z_impl_k_pipe_read(struct k_pipe *pipe, uint8_t *data, size_t len, k_timeout_t timeout)
{
	int rc;
	size_t read_len, first_chunk, second_chunk;

	__ASSERT_NO_MSG(pipe != NULL);
	__ASSERT_NO_MSG(data != NULL);

	k_spinlock_key_t key = k_spin_lock(&pipe->lock);
	if (utilization(pipe) == 0 && pipe->flags & PIPE_FLAG_OPEN) {
		rc = wait_for(&pipe->data, pipe, &key, queue_empty, timeout);
		if (rc && rc != -EPIPE) {
			k_spin_unlock(&pipe->lock, key);
			return rc;
		}
	}
	if (utilization(pipe) == 0 && !(pipe->flags & PIPE_FLAG_OPEN)) {
		k_spin_unlock(&pipe->lock, key);
		return -EPIPE;
	}

	read_len = MIN(len, utilization(pipe));
	first_chunk = MIN(pipe->buffer_size - pipe->head, read_len);
	second_chunk = read_len - first_chunk;

	memcpy(data, &pipe->buffer[pipe->head], first_chunk);

	if (second_chunk > 0) {
		memcpy(&data[first_chunk], pipe->buffer, second_chunk);
	}

	pipe->head = (pipe->head + read_len) % pipe->buffer_size;

	if (read_len) {
		pipe->flags &= ~PIPE_FLAG_FULL;
		notify_waiter(&pipe->space);
	}
	k_spin_unlock(&pipe->lock, key);
	return read_len;
}

int z_impl_k_pipe_reset(struct k_pipe *pipe)
{
	__ASSERT_NO_MSG(pipe != NULL);
	K_SPINLOCK(&pipe->lock) {
		pipe->head = 0U;
		pipe->tail = 0U;
		pipe->flags |= PIPE_FLAG_RESET;
		z_unpend_all(&pipe->data);
		z_unpend_all(&pipe->space);
	}

	return 0;
}

int z_impl_k_pipe_close(struct k_pipe *pipe)
{
	int rc = 0;

	__ASSERT_NO_MSG(pipe != NULL);
	K_SPINLOCK(&pipe->lock) {
		if (!(pipe->flags & PIPE_FLAG_OPEN)) {
			rc = -EALREADY;
			K_SPINLOCK_BREAK;
		}
		pipe->flags = 0;
		z_unpend_all(&pipe->data);
		z_unpend_all(&pipe->space);
	}
	return rc;
}
