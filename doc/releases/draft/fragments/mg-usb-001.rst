::area:: USB
::pr:: 107813

::migration-guide::
On STM32N6, the ``clocks`` cell which configures the USBPHYC clock mux has been moved
from :samp:`usbotg_hs{N}` to :samp:`usbphyc{N}` nodes at SoC DTSI level. Boards which
use an STM32N6 SoC with custom clock mux configuration must now set the ``clocks``
property on :samp:`usbphyc{N}` instead of :samp:`usbotg_hs{N}`.
