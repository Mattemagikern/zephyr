::area:: Build system
::pr:: 116673

::release-notes::
The :ref:`hardening tool <hardening>` (``west build -t hardenconfig``) now sources its
recommendations from a schema-validated YAML database instead of a CSV file: profiles in
:file:`scripts/kconfig/hardening.yaml` and per-subsystem ``hardening.yaml`` fragments living
next to the Kconfig files they relate to. Every recommendation now carries a rationale,
displayed in the report, and may reference CWE/CVE entries; recommendations are grouped into
profiles (``base`` and ``strict``, selectable with ``-DHARDENCONFIG_PROFILE=``); integer
recommendations can express minimum/maximum constraints; JSON output and a failing exit code
are available for CI use; and out-of-tree databases can be layered with
``-DHARDENCONFIG_EXTRA_SOURCES=``. The database is validated in CI against the actual Kconfig
tree so entries can no longer go stale.
