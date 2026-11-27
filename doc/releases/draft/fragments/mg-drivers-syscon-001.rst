::area:: Drivers: Syscon
::pr:: 106755

::migration-guide::
The syscon API functions :c:func:`syscon_read_reg` and :c:func:`syscon_write_reg` now use
``uint32_t`` for the register offset parameter instead of ``uint16_t``. This allows for
larger register offsets. Code that explicitly declares ``uint16_t`` variables for the
register parameter or implements the syscon driver API functions may need to be updated.
