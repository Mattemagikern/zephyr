::area:: Drivers: Audio
::pr:: 110631

::migration-guide::
The audio codec driver backend API now uses :c:struct:`audio_codec_driver_api` instead of
``struct audio_codec_api``.

Out-of-tree audio codec drivers must rename their backend API struct definitions and switch
their API instances to ``DEVICE_API(audio_codec, ...)``. See :github:`110631` for examples of how
in-tree drivers have been updated. Application code using the ``audio_codec_...`` APIs is not
impacted.
