::area:: SPDX Tooling
::pr:: 123537

::release-notes::
Software bill of materials (``west spdx``):

* SPDX 3.0 JSON-LD documents, selected with ``--spdx-version 3.0``, alongside the SPDX 2.3
  default; SPDX 3.1 output is experimental. Element IDs are unique to each build, so the SBOMs of
  the images of a :ref:`sysbuild` build can be merged into one graph. See
  :ref:`west-spdx-versions`.
* SPDX 3 documents carry the :ref:`Build profile <west-spdx-build-profile>`: the toolchain with
  its versions, the board and architecture, and a sub-build per library with its sources and
  compile flags.
* Zephyr and its modules are recorded with a supplier and a purl pinned to the revision that was
  built, and Zephyr with a CPE built from :zephyr_file:`VERSION`, release candidates included.
  The sources of each module are linked to its ``modules-deps`` entry. See
  :ref:`west-spdx-package-id`.
* Prebuilt libraries linked into the image, such as vendor blobs, are now listed, with the
  version and download URL their module declares. See :ref:`west-spdx-blobs`.
* Licenses and copyrights are resolved with the :command:`reuse` tool, so :file:`REUSE.toml` and
  :file:`.license` files are honored, including for binary files.
* Files are also hashed with SHA512, and each document records its author and the Zephyr version
  of the tool that generated it.
