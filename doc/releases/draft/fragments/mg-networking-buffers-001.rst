::area:: Networking Buffers
::pr:: 119052

::migration-guide::
:c:func:`net_buf_max_len` and :c:func:`net_buf_simple_max_len` have been deprecated. They
returned the capacity of the buffer behind its ``data`` pointer, which is neither the storage
size nor the room left for more data. Use :c:func:`net_buf_tailroom` or
:c:func:`net_buf_simple_tailroom` to find out how much data can still be added, and
:c:func:`net_buf_headroom` or :c:func:`net_buf_simple_headroom` for how much can be pushed in
front. Code that used the value as the size of a scratch area starting at ``data`` can
compute it as ``buf->len + net_buf_tailroom(buf)``.
