::area:: West project: lvgl
::pr:: 114213

::migration-guide::
The ``zephyr,lvgl-pointer-input`` devicetree binding marks the ``swap-xy``, ``invert-x``, and
``invert-y`` properties as **deprecated**. Users should instead add the corresponding
touchscreen properties ``swapped-x-y``, ``inverted-x``, and ``inverted-y`` to the underlying
touch input controller device node, where these transformations are now defined canonically.
