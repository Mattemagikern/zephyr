::area:: Utilities
::pr:: 117823

::migration-guide::
.. code-block:: c

   int write_pkg(struct ring_buf *rb, const uint8_t *payload, size_t payload_size)
   {
           struct hdr *h;
           uint8_t *ptr;
           uint32_t claim_size;

           /* Reserve the header region without committing it. */
           if (ring_buf_put_ptr(rb, (uint8_t **)&h, 0) < sizeof(*h)) {
                   return -ENOMEM;
           }

           /* Expose the region right after the header via a trailing offset. */
           claim_size = ring_buf_put_ptr(rb, &ptr, sizeof(*h));
           if (claim_size == 0) {
                   /* Nothing was committed to rb, so the write is cancelled. */
                   return -ENOMEM;
           }
           h->len = MIN(claim_size, payload_size);
           /* ... write payload through ptr ... */

           /* Publish header and payload atomically to the real buffer. */
           ring_buf_commit(rb, sizeof(*h) + h->len);
           return h->len;
   }
