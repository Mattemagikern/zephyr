::area:: Multimedia Pipeline
::pr:: 120595

::release-notes::
Introducing :ref:`mpipe`, an experimental subsystem for building multimedia
applications out of reusable elements - sources, transforms and sinks -
linked together into a pipeline. It lets an application describe the media
flow it wants instead of driving each audio, video or display device itself.

The base plugin adds the media-agnostic elements: a caps filter, a tee, a
queue, and an application source and sink.

A topology dump renders a pipeline as a Graphviz graph, and a player drives
a pipeline from a worker thread and the shell.
