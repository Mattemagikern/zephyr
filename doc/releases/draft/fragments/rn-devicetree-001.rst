::area:: Devicetree
::pr:: 108892

::release-notes::
Nodes can now use phandles to refer to their children without causing a cycle in the
dependency graph and a build error. See :ref:`dt-bindings-dependency-mode` how to
use this new feature.

* :c:macro:`DT_NODELABEL_C_TOKEN`
* :c:macro:`DT_NODELABEL_C_TOKEN_BY_IDX`
