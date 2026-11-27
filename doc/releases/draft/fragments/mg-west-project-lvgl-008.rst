::area:: West project: lvgl
::pr:: 104733

::migration-guide::
:kconfig:option:`CONFIG_LV_Z_FULL_REFRESH` is now part of the ``LV_Z_RENDERING_MODE`` Kconfig
choice, alongside :kconfig:option:`CONFIG_LV_Z_PARTIAL_REFRESH` (default) and
:kconfig:option:`CONFIG_LV_Z_DIRECT_RENDERING`. Setting ``CONFIG_LV_Z_FULL_REFRESH=y`` in a
``.conf`` fragment still works, but ``CONFIG_LV_Z_FULL_REFRESH=n`` is silently ignored because
a choice member cannot be deselected that way. Out-of-tree boards or shields that used
``CONFIG_LV_Z_FULL_REFRESH=n`` to opt out of a full-refresh default must instead override the
choice default in a ``Kconfig.defconfig`` or ``.defconfig`` file:

.. code-block:: kconfig

   choice LV_Z_RENDERING_MODE
     default LV_Z_PARTIAL_REFRESH
   endchoice
