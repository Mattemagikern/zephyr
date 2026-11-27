::area:: ARM arch
::pr:: 113664

::release-notes::
Arm:

* The non-secure variant of
    :zephyr:board:`Arm Musca-S1 <v2m_musca_s1>` (``v2m_musca_s1/musca_s1/ns``)
    has been removed due to TF-M removing platform support for this board.
* As a consequence of the above, the secure variant of
  :zephyr:board:`Arm Musca-S1 <v2m_musca_s1>` (``v2m_musca_s1``) has been deprecated.
  This is to avoid a confusing state of partial support.
