::area:: Storage
::pr:: 116744

::migration-guide::
``stream_flash_erase_page()`` has been removed. Use :c:func:`flash_area_erase` or
:c:func:`flash_erase` instead; there is no Stream Flash API equivalent.
