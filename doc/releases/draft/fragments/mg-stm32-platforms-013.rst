::area:: STM32 Platforms
::pr:: 114092

::migration-guide::
Properties ``wkup-pins-nb``, ``wkup-pins-srcs``, ``wkup-pins-pol``, and ``wkup-pins-pupd`` as well as
the child binding of :dtcompatible:`st,stm32-pwr`, which were related to wake-up pins, have been removed.
As a replacement, a node named ``wakeup-controller`` with new compatible :dtcompatible:`st,stm32-pwr-wkupctrl`
is introduced as a child node to all existing :dtcompatible:`st,stm32-pwr` nodes.

For most out-of-tree users, it is sufficient to move the ``status = "okay";`` property along with wake-up pin
nodes declared in board DTS (if any) from the ``&pwr`` node to its new child named ``wakeup-controller``.
The following Devicetree snippets show how this can be achieved by adding two lines in board DTS:

.. tabs::

  .. group-tab:: Before

    .. code-block:: devicetree

        &pwr {
          wkup-pin@1 {
            /* ... */
          };

          status = "okay";
        };

  .. group-tab:: After

    .. code-block:: devicetree
      :emphasize-lines: 2, 8

        &pwr {
          wakeup-controller {
            wkup-pin@1 {
              /* ... */
            };

            status = "okay";
          };
        };

Note that wake-up pin nodes are now called :samp:`wkup@{N}` instead of :samp:`wkup-pin@{N}` in tree.
This change is cosmetic and has no functional impact.

:dtcompatible:`st,stm32-pwr` nodes are now enabled by default by SoC DTSI as the ``status = "disabled";``
property has been removed. This should have no impact since the property was not used except for the
wake-up pins feature, which is now handled by :dtcompatible:`st,stm32-pwr-wkupctrl`.
