# SPDX-FileCopyrightText: Måns Ansgariusson <mansgariusson@gmail.com>
# SPDX-License-Identifier: Apache-2.0

from pathlib import Path

import release_fragments
from docutils.statemachine import StringList
from release_fragments import (
    DOCUMENTS,
    MAINTAINERS_FILE,
    SECTIONS_FILE,
    fragment_files,
    load_config,
    load_fragments,
    render,
)
from sphinx.util.docutils import SphinxDirective, switch_source_input
from sphinx.util.parsing import nested_parse_to_nodes


def fragment_relpaths(fragments_dir, srcdir):
    return tuple(path.relative_to(srcdir).as_posix() for path in fragment_files(fragments_dir))


def run_release_fragments(directive):
    env = directive.env
    document = directive.arguments[0]
    if document not in DOCUMENTS:
        raise directive.error(f"unknown document {document!r}, expected one of {tuple(DOCUMENTS)}")

    source = Path(env.doc2path(env.docname))
    fragments_dir = source.parent / "fragments"
    relpaths = fragment_relpaths(fragments_dir, env.srcdir)

    env.metadata.setdefault(env.docname, {})["release_fragments"] = relpaths
    code = (__file__, release_fragments.__file__)
    for dependency in (*code, MAINTAINERS_FILE, SECTIONS_FILE, *relpaths):
        env.note_dependency(str(dependency))

    try:
        grouped = load_fragments(fragments_dir, *load_config())
    except (OSError, ValueError) as err:
        raise directive.error(str(err)) from err

    content = StringList()
    for text, line_source, lineno in render(grouped, document, source.parent.name):
        if line_source is None:
            line_source, lineno = source, directive.lineno
        content.append(text, str(line_source), max(lineno - 1, 0))

    with switch_source_input(directive.state, content):
        return nested_parse_to_nodes(directive.state, content, allow_section_headings=True)


class ReleaseFragmentsDirective(SphinxDirective):
    required_arguments = 1
    run = run_release_fragments


def get_outdated(app, env, added, changed, removed):
    outdated = set()

    for docname, metadata in env.metadata.items():
        previous = metadata.get("release_fragments")
        if previous is None or docname not in env.found_docs:
            continue
        fragments_dir = Path(env.doc2path(docname)).parent / "fragments"
        if previous != fragment_relpaths(fragments_dir, env.srcdir):
            outdated.add(docname)

    return outdated


def setup(app):
    app.add_directive("release-fragments", ReleaseFragmentsDirective)
    app.connect("env-get-outdated", get_outdated)

    return {
        "version": "0.3",
        "parallel_read_safe": True,
        "parallel_write_safe": True,
    }
