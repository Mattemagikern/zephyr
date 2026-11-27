::area:: Drivers: RTC
::pr:: 95221

::migration-guide::
The legacy counter-based DS3231 driver has been removed, completing the deprecation introduced in
:github:`95221`. Applications using :dtcompatible:`maxim,ds3231`, ``CONFIG_COUNTER_MAXIM_DS3231``,
or :file:`<zephyr/drivers/rtc/maxim_ds3231.h>` must migrate to the RTC subsystem driver.

Replace the single legacy I2C node with a :dtcompatible:`maxim,ds3231-mfd` parent and a
:dtcompatible:`maxim,ds3231-rtc` child. Move ``isw-gpios`` to the RTC child, rename old
``32k-gpios`` usage to ``freq-32khz-gpios``, and replace ``maxim_ds3231_*`` helper API usage with
generic RTC subsystem APIs.
