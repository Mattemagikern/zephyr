.. _memref:

Shared Memory References
########################

A :dfn:`memref` is a reference counted allocation that can have several simultaneous owners.
It is intended for heap allocated objects that cross execution context boundaries, for example
message payloads shared between a producer and multiple consumer threads. The producer allocates a
message, hands it to any number of consumers and releases its own reference. Each consumer that
keeps the message takes its own reference and releases it when done. The last release frees the
block, so no context needs to know how many others still hold it.

.. contents::
   :local:
   :depth: 2

Overview
********

A plain ``malloc`` and ``free`` pair cannot express shared ownership.
When several contexts need to hold the same block, the last owner to finish must release it, which
otherwise requires hand written accounting at every handoff or assumptions about the lifetime of the
block.

The memref API adds a small control block in front of each allocation. The control block holds an
atomic reference count, an optional destroy callback and the backend the block came from, so the
returned pointer carries its own free path and can cross module boundaries without the receiver
knowing how it was allocated.

Concepts
********

Each allocation starts with a reference count of one.
Any other context that keeps the pointer takes ownership first with :c:func:`memref_ref` and
releases it later with :c:func:`memref_unref`.
When the count reaches zero the optional destroy callback runs and the block is returned to its
backend.

.. mermaid::
   :caption: Lifecycle of a memref allocation
   :alt: State diagram of a memref allocation. memref_alloc moves it to the owned state with a
         reference count of one. memref_ref raises the count and memref_unref lowers it while the
         count stays above zero. When memref_unref drops the count to zero the destroy callback
         runs, if set, and the block is returned to its backend.

   stateDiagram-v2
       direction LR
       [*] --> Owned : memref_alloc() / count = 1
       Owned --> Owned : memref_ref() / count + 1
       Owned --> Owned : memref_unref() / count - 1, count > 0
       Owned --> Releasing : memref_unref() / count = 0
       Releasing --> [*] : destroy callback (optional), then backend free

The control block is private. The pointer returned to the caller addresses only the user payload,
padded so the backend alignment guarantee is kept. Size addition overflow and zero size requests
are rejected with ``NULL``.

.. mermaid::
   :caption: Byte layout of a memref allocation on a 32-bit target with 8-byte maximum
             alignment, ``CONFIG_MEMREF_DEBUG`` and ``CONFIG_MEMREF_DESTROY_CB`` enabled
   :alt: Packet diagram of a memref allocation in bytes. Bytes 0 to 3 hold the reference count,
         bytes 4 to 7 the magic value, bytes 8 to 11 the destroy callback pointer and bytes 12 to
         15 the backend pointer. The user payload starts at byte 16, which is the pointer returned
         by memref_alloc.

   ---
   config:
     packet:
       bitOrder: descending
       bitsPerRow: 16
       bitWidth: 48
   ---
   packet
     0-3: "Reference count"
     4-7: "Magic"
     8-11: "Destroy callback"
     12-15: "Backend pointer"
     16-31: "User payload (pointer returned by memref_alloc)"

The magic value is present only with :kconfig:option:`CONFIG_MEMREF_DEBUG` and the destroy callback
only with :kconfig:option:`CONFIG_MEMREF_DESTROY_CB`. The control block is padded to the maximum
alignment, so on such a target it occupies 16 bytes with either option enabled and 8 bytes with
both disabled. On 64-bit targets the reference count and the pointers are 8 bytes wide.
:c:func:`memref_ref` and :c:func:`memref_unref` locate the control block by stepping back a fixed,
alignment-rounded distance from the user pointer.

Backends
********

Raw storage comes from a backend with allocate and free callbacks plus an opaque context pointer.
There are two flavours. A variable length backend, :c:struct:`memref_backend`, honours the
requested allocation size.

A fixed block size backend, :c:struct:`memref_block_backend`, wraps a variable length backend whose
underlying allocator serves fixed size blocks decided at compile or initialization time, such as a
:c:struct:`k_mem_slab`.
The distinct type makes calling :c:func:`memref_alloc` on a block backend, or
:c:func:`memref_block_alloc` on a variable length backend, a compile time error.

The backend stays attached to the allocation, so :c:func:`memref_ref` and :c:func:`memref_unref`
only take the user pointer and behave identically regardless of flavour. Pointers from different
backends can be passed between modules without tracking their origin. The backend must remain
valid for the lifetime of every allocation made through it, and a memref backend inherits the
limitations of its memory allocator.

Defining a backend
==================

For the common allocators, define a backend with one of the ready-made macros:

.. code-block:: c

   #include <zephyr/sys/memref.h>

   /* Variable length: defines the heap and the matching backend */
   MEMREF_K_HEAP_DEFINE(pool_backend, 4096);

   /* Fixed block size: defines the slab and the matching block backend. The
    * slab block size accounts for the control block; 128 is the user payload.
    */
   MEMREF_K_MEM_SLAB_DEFINE(slab_backend, 128, 10, sizeof(void *));

To wrap any other allocator, publish a backend with :c:macro:`MEMREF_BACKEND_DEFINE` or
:c:macro:`MEMREF_BLOCK_BACKEND_DEFINE`:

.. code-block:: c

   static void *heap_alloc(void *ctx, size_t size)
   {
       return k_heap_alloc((struct k_heap *)ctx, size, K_NO_WAIT);
   }

   static void heap_free(void *ctx, void *mem)
   {
       k_heap_free((struct k_heap *)ctx, mem);
   }

   K_HEAP_DEFINE(pool, 4096);
   MEMREF_BACKEND_DEFINE(pool_backend, heap_alloc, heap_free, &pool);

A fixed block size backend provides the same ``alloc`` signature, but its ``alloc`` function
disregards the size argument as the block size is fixed at compile or initialization time.

Usage
*****

Allocating and releasing
========================

A receiver that keeps the pointer beyond the call takes a reference:

.. code-block:: c

   void broadcast_to_consumers(struct message *msg) {
       for (int i = 0; i < NUM_CONSUMERS; i++) {
           consumer[i].call_back(msg); // consumer can choose to take shared ownership before return
       }
   }

   ... {
       struct message *msg = memref_alloc(backend, sizeof(*msg));
       ....
       broadcast_to_consumers(msg);
       memref_drop(msg); /* Release the sender's reference and clear msg */
   }

Each consumer then calls :c:func:`memref_ref` if they want to keep the reference and process it
later.
If a consumer makes an ownership claim (calls :c:func:`memref_ref`) it must eventually release it
with :c:macro:`memref_drop` once it is done with the block.
The block is reclaimed once the sender and every consumer have released it.

Prefer :c:macro:`memref_drop` over :c:func:`memref_unref` to release a reference held in a
variable. It releases the reference and sets the variable to ``NULL``, so a later use of the
released pointer is a ``NULL`` dereference instead of a use after free, and a second release is a
no-op. Use :c:func:`memref_unref` when the pointer is not held in a modifiable variable.

.. mermaid::
   :caption: Ownership handoff between a producer and two consumers
   :alt: Sequence diagram. The producer calls memref_alloc, which takes storage from the backend
         and returns the payload with a count of one. The producer hands the pointer to two
         consumers, each of which calls memref_ref, raising the count to three. The producer then
         calls memref_unref, lowering the count to two. Each consumer calls memref_unref after
         processing. The last call drops the count to zero, runs the destroy callback if set and
         returns the block to the backend.

   sequenceDiagram
       participant P as Producer
       participant M as memref
       participant B as Backend
       participant C1 as Consumer 1
       participant C2 as Consumer 2

       P->>M: memref_alloc(backend, size)
       M->>B: alloc(ctx, CTRL_BLOCK_SIZE + size)
       B-->>M: raw block
       M-->>P: payload pointer (count = 1)
       P->>C1: hand off pointer
       C1->>M: memref_ref() (count = 2)
       P->>C2: hand off pointer
       C2->>M: memref_ref() (count = 3)
       P->>M: memref_unref() (count = 2)
       C1->>M: memref_unref() (count = 1)
       C2->>M: memref_unref() (count = 0)
       opt destroy callback set
           M->>M: destroy(payload)
       end
       M->>B: free(ctx, block)

Allocations from a fixed block size backend use :c:func:`memref_block_alloc`, which takes no size
argument. Everything else, including :c:func:`memref_ref` and :c:func:`memref_unref`, is identical.

Destroy callback
----------------

A :c:type:`memref_destroy_cb_t` passed at allocation time runs with the user pointer just before the
block returns to the backend when the last reference is dropped.
It is passed to :c:func:`memref_alloc_with_destroy` or :c:func:`memref_block_alloc_with_destroy`
instead of :c:func:`memref_alloc` or :c:func:`memref_block_alloc`, and requires
:kconfig:option:`CONFIG_MEMREF_DESTROY_CB`. With the option disabled the ``_with_destroy`` variants
are not defined and such a call fails to build.

Use the callback to release resources tied to the payload:

.. code-block:: c

   static void msg_destroy(void *mem)
   {
       struct message *msg = mem;

       k_free(msg->extra);
   }

   struct message *msg = memref_alloc_with_destroy(&pool_backend, sizeof(*msg), msg_destroy);

Execution Contexts
******************

:c:func:`memref_ref` and :c:func:`memref_unref` inherit the execution context of the backend
callbacks and the destroy callback.
Do not call them from ISR unless the backend and the destroy callback of that allocation are
documented as ISR safe.
Holding a reference is a precondition of :c:func:`memref_unref`, and :c:func:`memref_ref` must not
be used on a pointer whose count already reached zero.

Debugging
*********

:kconfig:option:`CONFIG_MEMREF_DEBUG` stores a magic value in every control block and checks it in
:c:func:`memref_ref` and :c:func:`memref_unref`. Passing a pointer that was not allocated through
memref, or one whose last reference was already released, then fails an assertion. The option
requires :kconfig:option:`CONFIG_ASSERT` and is disabled by default, so enabling assertions does
not grow the control block on its own.

The check is best effort. The magic value is cleared when the last reference is released, but a
later call reads memory that has already been returned to the backend and may have been reused.
The assertions belong to the ``DEFAULT`` assert module and are compiled out if its level is off.

Sample
******

The :zephyr:code-sample:`memref` sample (:zephyr_file:`samples/data_structures/memref`) demonstrates
the API on a ``k_heap`` backend. A producer allocates a message, shares ownership with three
consumer threads through message queues, and releases its own reference, while each consumer
releases its reference after processing.

Configuration
*************

Related configuration options:

* :kconfig:option:`CONFIG_MEMREF`
* :kconfig:option:`CONFIG_MEMREF_DESTROY_CB`
* :kconfig:option:`CONFIG_MEMREF_DEBUG`

API Reference
*************

.. doxygengroup:: memref
