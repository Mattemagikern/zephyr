::area:: Utilities
::pr:: 107526

::migration-guide::
The :c:func:`ZTEST_BENCHMARK_SETUP_TEARDOWN` and :c:func:`ZTEST_BENCHMARK_TIMED_SETUP_TEARDOWN` macros have
been removed. Their setup/teardown signature has been folded into :c:func:`ZTEST_BENCHMARK` and
:c:func:`ZTEST_BENCHMARK_TIMED`, which now require explicit ``setup_fn`` and ``teardown_fn``
arguments at every call site. Pass ``NULL`` when a benchmark genuinely needs neither.

Update existing call sites as follows:

.. code-block:: c

   /* Before */
   ZTEST_BENCHMARK(suite, my_bench, 100) { /* ... */ }
   ZTEST_BENCHMARK_TIMED(suite, my_bench, 1000) { /* ... */ }
   ZTEST_BENCHMARK_SETUP_TEARDOWN(suite, my_bench, 100, setup, teardown) { /* ... */ }
   ZTEST_BENCHMARK_TIMED_SETUP_TEARDOWN(suite, my_bench, 1000, setup, teardown) { /* ... */ }

   /* After */
   ZTEST_BENCHMARK(suite, my_bench, 100, NULL, NULL) { /* ... */ }
   ZTEST_BENCHMARK_TIMED(suite, my_bench, 1000, NULL, NULL) { /* ... */ }
   ZTEST_BENCHMARK(suite, my_bench, 100, setup, teardown) { /* ... */ }
   ZTEST_BENCHMARK_TIMED(suite, my_bench, 1000, setup, teardown) { /* ... */ }
