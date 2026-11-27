::area:: Utilities
::pr:: 107437

::migration-guide::
Demand paging (``subsys/demand_paging``) is moved under Memory Management
into ``subsys/mem_mgmt/demand_paging``. Custom backing store and eviction algorithm code need
to be moved there.
