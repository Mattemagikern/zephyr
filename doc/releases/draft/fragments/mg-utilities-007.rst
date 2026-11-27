::area:: Utilities
::pr:: 117823

::migration-guide::
.. code-block:: c

   int write_pkg(struct ring_buf *rb, const uint8_t *payload, size_t payload_size)
   {
           struct hdr *h;
           uint8_t *ptr;
           uint32_t claim_size;

           claim_size = ring_buf_put_claim(rb, (uint8_t **)&h, sizeof(*h));
           if (claim_size < sizeof(*h)) {
                   ring_buf_put_finish(rb, 0);
                   return -ENOMEM;
           }

           claim_size = ring_buf_put_claim(rb, &ptr, payload_size);
           if (claim_size == 0) {
                   ring_buf_put_finish(rb, 0);
                   return -ENOMEM;
           }
           h->len = claim_size;
           /* ... write payload through ptr ... */
           ring_buf_put_finish(rb, sizeof(*h) + h->len);
           return h->len;
   }
