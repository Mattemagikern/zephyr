::area:: Drivers: Video
::pr:: 112969

::migration-guide::
:c:func:`video_import_buffer` no longer returns the imported buffer index via a
``uint16_t *idx`` output parameter but instead returns a pointer to the imported
:c:struct:`video_buffer`, or ``NULL`` on failure. This helps to make the index transparent
to the application and also makes the buffer accessible from the application.
