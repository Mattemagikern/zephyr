#!/usr/bin/env python3
# SPDX-FileCopyrightText: The Zephyr Project Contributors
# SPDX-License-Identifier: Apache-2.0

import argparse
import difflib
import re
import shutil
import subprocess
import sys
import tempfile
from fnmatch import fnmatchcase
from pathlib import Path

import yaml

ZEPHYR_BASE = Path(__file__).resolve().parents[2]
RELEASES_DIR = ZEPHYR_BASE / "doc" / "releases"
TEMPLATE_DIR = RELEASES_DIR / "template"
DRAFT_DIR = RELEASES_DIR / "draft"
MAINTAINERS_FILE = ZEPHYR_BASE / "MAINTAINERS.yml"
SECTIONS_FILE = RELEASES_DIR / "sections.yml"
INVENTORY_SCRIPT = ZEPHYR_BASE / "scripts" / "release" / "gen_change_inventory.py"
RELEASES_INDEX = RELEASES_DIR / "index.rst"
DRAFT_TOCTREE_ENTRY = "   draft/index\n"

DOCUMENTS = {
    "release-notes": "release notes",
    "migration-guide": "migration guide",
}
CURATED = ("highlights.rst.inc", "security-notice.rst.inc")
INVENTORY = "change-inventory"
META_TAGS = ("area", "pr")
TAG_PATTERN = re.compile(r"^::\s*([\w-]+)\s*::", re.MULTILINE)
PR_PATTERN = re.compile(r"#?(\d+)")
NON_PARAGRAPH = re.compile(r"\s|\.\.|[*+-]\s|#\.\s|\d+\.\s|[=+|]|:[^:`\s][^:`]*:(\s|$)")
DIRECTIVE = ".. release-fragments::"
BLANK = ("", None, 0)


def display_path(path):
    path = Path(path).resolve()
    return path.relative_to(ZEPHYR_BASE) if path.is_relative_to(ZEPHYR_BASE) else path


def fragment_error(path, line, message):
    error = ValueError(f"{path}:{line}: {message}")
    error.location = (Path(path), line, message)
    return error


def line_of(text, offset):
    return text.count("\n", 0, offset) + 1


def is_comment_only(text):
    lines = [line.strip() for line in text.splitlines() if line.strip()]
    return all(line.startswith("..") and "::" not in line for line in lines)


def read_blocks(path):
    text = Path(path).read_text(encoding="utf-8")
    matches = list(TAG_PATTERN.finditer(text))
    blocks = {}

    for match, following in zip(matches, [*matches[1:], None], strict=True):
        tag = match.group(1).lower()
        line = line_of(text, match.start())
        if tag not in META_TAGS and tag not in DOCUMENTS:
            raise fragment_error(path, line, f"unknown tag '::{tag}::'")
        if tag in blocks:
            raise fragment_error(path, line, f"duplicate '::{tag}::'")

        raw = text[match.end() : following.start() if following else len(text)]
        body = raw.strip()
        lead = len(raw) - len(raw.lstrip())
        blocks[tag] = (body, line_of(text, match.end() + lead) if body else line)

    return blocks


def parse_fragment(path):
    blocks = read_blocks(path)
    fragment = {"path": Path(path)}

    for tag in META_TAGS:
        text, line = blocks.pop(tag, ("", 1))
        if not text:
            raise fragment_error(path, line, f"missing or empty '::{tag}::'")
        if "\n" in text:
            raise fragment_error(path, line, f"'::{tag}::' must be a single line")
        fragment[tag] = text
        fragment[f"{tag}_line"] = line

    prs = fragment["pr"].replace(",", " ").split()
    if not all(PR_PATTERN.fullmatch(pr) for pr in prs):
        raise fragment_error(path, fragment["pr_line"], "'::pr::' must list PR numbers, e.g. 12345")
    fragment["pr"] = [PR_PATTERN.fullmatch(pr).group(1) for pr in prs]

    fragment["content"] = {
        tag: block for tag, block in blocks.items() if not is_comment_only(block[0])
    }
    if not fragment["content"]:
        raise fragment_error(path, 1, f"no content in any of {', '.join(DOCUMENTS)}")

    return fragment


def load_yaml(path):
    try:
        return yaml.safe_load(Path(path).read_text(encoding="utf-8"))
    except yaml.YAMLError as err:
        raise ValueError(f"{path}: {err}") from err


def load_areas(path=MAINTAINERS_FILE):
    data = load_yaml(path)
    if not isinstance(data, dict):
        raise ValueError(f"{path}: expected a mapping of areas")
    return set(data)


SECTION_KEYS = {"areas", "subsections", "split"}


def parse_section(title, value):
    if isinstance(value, list):
        return title, value, []
    unknown = set(value) - SECTION_KEYS
    if unknown:
        raise ValueError(f"section {title!r}: unknown keys {sorted(unknown)}")
    named = list(value.get("subsections", {}).items())
    return title, value.get("areas", []), [*named, (None, value.get("split", []))]


def load_sections(path=SECTIONS_FILE):
    data = load_yaml(path)
    try:
        extra_areas = set(data.get("extra-areas", []))
        sections = [parse_section(title, value) for title, value in data["sections"].items()]
    except (AttributeError, KeyError, TypeError, ValueError) as err:
        raise ValueError(f"{path}: malformed sections file ({err})") from err
    return sections, extra_areas


def load_config():
    sections, extra_areas = load_sections()
    return load_areas() | extra_areas, sections


def fragment_files(fragments_dir):
    return sorted(
        path for path in Path(fragments_dir).glob("*.rst") if not path.name.startswith("_")
    )


def matches_any(area, patterns):
    return any(fnmatchcase(area.casefold(), pattern.casefold()) for pattern in patterns)


def derived_subsection(area, section):
    name = area.split(": ", 1)[-1]
    return name.removeprefix(f"{section} ") if name != section else name


def placement_of(fragment, areas, sections):
    area = fragment["area"]
    path = fragment["path"]
    line = fragment["area_line"]

    if area not in areas:
        matches = difflib.get_close_matches(area, sorted(areas), n=3)
        hint = f" (did you mean {', '.join(map(repr, matches))}?)" if matches else ""
        raise fragment_error(path, line, f"unknown area {area!r}{hint}")

    for title, section_areas, subsections in sections:
        for subtitle, subsection_areas in subsections:
            if matches_any(area, subsection_areas):
                return title, subtitle or derived_subsection(area, title)
        if matches_any(area, section_areas):
            return title, None

    sections_file = SECTIONS_FILE.relative_to(ZEPHYR_BASE)
    raise fragment_error(path, line, f"area {area!r} is not mapped to a section in {sections_file}")


def load_fragments(fragments_dir, areas, sections):
    grouped = {}
    for path in fragment_files(fragments_dir):
        fragment = parse_fragment(path)
        section, subsection = placement_of(fragment, areas, sections)
        grouped.setdefault(section, {}).setdefault(subsection, []).append(fragment)

    return {
        section: dict(sorted(subsections.items(), key=lambda item: subsection_key(item[0])))
        for section, subsections in sorted(grouped.items(), key=lambda item: item[0].casefold())
    }


def subsection_key(subsection):
    return (subsection is not None, (subsection or "").casefold())


def sort_key(fragment):
    return fragment["area"].casefold(), fragment["path"].name


def label_of(fragment, document, prefix):
    return f"{prefix}-{fragment['path'].stem}-{document}"


def heading(text, char):
    return [(text, None, 0), (char * len(text), None, 0), BLANK]


def ends_with_paragraph(text):
    last_block = re.split(r"\n\s*\n", text)[-1]
    indented = any(line[:1].isspace() for line in last_block.splitlines())
    return not indented and not NON_PARAGRAPH.match(last_block)


def linked_documents(fragment, document):
    return [other for other in DOCUMENTS if other != document and other in fragment["content"]]


def trailer_of(prs, see_also):
    pieces = [", ".join(f":github:`{pr}`" for pr in prs)]
    for index, (other, label) in enumerate(see_also.items()):
        ref = f":ref:`{DOCUMENTS[other]} <{label}>`"
        pieces.append(f"{'see' if index == 0 else 'and'} the {ref}")
    if see_also:
        pieces[0] += ";"
    pieces[0] = f"({pieces[0]}"
    pieces[-1] += ")"
    return pieces


def render_entry(fragments, document, prefix):
    body = []
    see_also = {}

    for index, fragment in enumerate(fragments):
        path = fragment["path"]
        text, first_line = fragment["content"][document]
        others = linked_documents(fragment, document)
        if index:
            body.append(BLANK)
        if others:
            body += [(f".. _{label_of(fragment, document, prefix)}:", path, first_line), BLANK]
        for other in others:
            see_also.setdefault(other, label_of(fragment, other, prefix))
        body += [(line, path, first_line + offset) for offset, line in enumerate(text.splitlines())]

    last = fragments[-1]
    text, first_line = last["content"][document]
    if not ends_with_paragraph(text):
        body.append(BLANK)
    last_line = first_line + text.count("\n")
    body += [(line, last["path"], last_line) for line in trailer_of(last["pr"], see_also)]

    indented = [
        (f"  {line}" if line.strip() else "", source, lineno) for line, source, lineno in body
    ]
    first_text, first_source, first_lineno = indented[0]
    return [(f"* {first_text[2:]}", first_source, first_lineno), *indented[1:], BLANK]


def group_by_pr(fragments):
    groups = {}
    for fragment in sorted(fragments, key=sort_key):
        groups.setdefault(tuple(fragment["pr"]), []).append(fragment)
    return list(groups.values())


def render(grouped, document, prefix="draft"):
    lines = []

    for section, subsections in grouped.items():
        section_lines = []
        for subsection, fragments in subsections.items():
            entries = group_by_pr(f for f in fragments if document in f["content"])
            if not entries:
                continue
            if subsection:
                section_lines += heading(subsection, "=")
            for entry in entries:
                section_lines += render_entry(entry, document, prefix)
        if section_lines:
            lines += heading(section, "*") + section_lines

    return lines


def render_text(lines):
    return "".join(f"{text}\n" for text, _, _ in lines)


def version_arg(value):
    if not re.fullmatch(r"\d+\.\d+\.\d+", value):
        raise argparse.ArgumentTypeError(f"expected MAJOR.MINOR.PATCH, got {value!r}")
    return value


def read_versions(draft_dir):
    path = Path(draft_dir) / "version.rst.inc"
    text = path.read_text(encoding="utf-8")
    versions = dict(re.findall(r"^\.\. \|(\w+)\| replace:: (\S+)$", text, re.MULTILINE))
    if "version" not in versions or "last_release" not in versions:
        raise ValueError(f"{path}: expected |version| and |last_release| substitutions")
    return versions["version"], versions["last_release"]


def write_versions(draft_dir, version, last_release):
    path = Path(draft_dir) / "version.rst.inc"
    text = f".. |version| replace:: {version}\n.. |last_release| replace:: {last_release}\n"
    path.write_text(text, encoding="utf-8")


def stray_fragment_dirs():
    return sorted(
        path for path in RELEASES_DIR.glob("*/fragments") if path != DRAFT_DIR / "fragments"
    )


def check_fragments(draft_dir=DRAFT_DIR, dirs=None):
    areas, sections = load_config()
    problems = []

    if not dirs:
        for directory in stray_fragment_dirs():
            for path in fragment_files(directory):
                target = display_path(Path(draft_dir) / "fragments")
                problems.append((path, 1, f"fragments belong in {target}"))

    for directory in dirs or [Path(draft_dir) / "fragments"]:
        for path in fragment_files(directory):
            try:
                placement_of(parse_fragment(path), areas, sections)
            except ValueError as err:
                problems.append(getattr(err, "location", (path, 1, str(err))))

    return problems


def cmd_check(args):
    problems = check_fragments(args.draft, args.dirs)
    for path, line, message in problems:
        print(f"{display_path(path)}:{line}: {message}", file=sys.stderr)
    return 1 if problems else 0


def cmd_render(args):
    grouped = load_fragments(args.draft / "fragments", *load_config())
    for document in args.document or DOCUMENTS:
        print(render_text(render(grouped, document)), end="")


def freeze_document(path, document, grouped, prefix):
    text = path.read_text(encoding="utf-8")
    directive = f"{DIRECTIVE} {document}\n"
    if directive not in text:
        raise ValueError(f"{path}: '{directive.strip()}' not found")
    rendered = render_text(render(grouped, document, prefix))
    path.write_text(text.replace(directive, rendered).rstrip("\n") + "\n", encoding="utf-8")


def generate_inventory(start, end, outdir, title, label):
    command = [sys.executable, str(INVENTORY_SCRIPT), "--title", title, "--label", label]
    subprocess.run([*command, start, end, str(outdir)], check=True, cwd=ZEPHYR_BASE)


def write_inventory_placeholder(directory, series):
    if directory.exists():
        shutil.rmtree(directory)
    directory.mkdir(parents=True)
    text = (TEMPLATE_DIR / INVENTORY / "index.rst").read_text(encoding="utf-8")
    (directory / "index.rst").write_text(text.replace("@SERIES@", series), encoding="utf-8")


def cmd_inventory(args):
    version, last_release = read_versions(args.draft)
    series = version.rsplit(".", 1)[0]
    output = args.draft / INVENTORY
    with tempfile.TemporaryDirectory(dir=args.draft, prefix=".inventory-") as staging:
        generated = Path(staging) / INVENTORY
        generate_inventory(
            args.start or f"v{last_release}",
            args.end,
            generated,
            f"Zephyr {series} change inventory (Working Draft)",
            "change_inventory_draft",
        )
        if output.exists():
            shutil.rmtree(output)
        generated.rename(output)
    print(f"regenerated {display_path(output)}")
    return 0


def legacy_documents(series):
    names = (f"release-notes-{series}.rst", f"migration-guide-{series}.rst")
    return [RELEASES_DIR / name for name in names if (RELEASES_DIR / name).exists()]


def add_to_releases_index(series):
    text = RELEASES_INDEX.read_text(encoding="utf-8")
    entry = f"   zephyr-{series}/index\n"
    RELEASES_INDEX.write_text(
        text.replace(DRAFT_TOCTREE_ENTRY, DRAFT_TOCTREE_ENTRY + entry, 1), encoding="utf-8"
    )


def cmd_freeze(args):
    draft = args.draft
    version, last_release = read_versions(draft)
    series = version.rsplit(".", 1)[0]
    output = args.output or RELEASES_DIR / f"zephyr-{series}"
    if output.exists():
        sys.exit(f"{output} already exists")

    in_tree = output.resolve().parent == RELEASES_DIR.resolve()
    if in_tree:
        legacy = legacy_documents(series)
        if legacy:
            names = ", ".join(display_path(path).as_posix() for path in legacy)
            sys.exit(f"Zephyr {series} is already documented in {names}: its labels would clash")
        if DRAFT_TOCTREE_ENTRY not in RELEASES_INDEX.read_text(encoding="utf-8"):
            sys.exit(f"{display_path(RELEASES_INDEX)}: toctree entry 'draft/index' not found")

    grouped = load_fragments(draft / "fragments", *load_config())

    shutil.copytree(TEMPLATE_DIR, output, ignore=shutil.ignore_patterns("fragment.template"))
    try:
        for path in output.rglob("*.rst*"):
            text = path.read_text(encoding="utf-8")
            text = text.replace("@VERSION@", version)
            text = text.replace("@LAST_VERSION@", last_release)
            text = text.replace("@SERIES@", series)
            path.write_text(text, encoding="utf-8")
        for name in CURATED:
            shutil.copyfile(draft / name, output / name)
        for document in DOCUMENTS:
            freeze_document(output / f"{document}.rst", document, grouped, f"zephyr-{series}")
        if not args.no_inventory:
            generate_inventory(
                args.inventory_start or f"v{last_release}",
                args.inventory_end,
                output / "change-inventory",
                f"Zephyr {series} change inventory",
                f"change_inventory_{series}",
            )
    except BaseException:
        shutil.rmtree(output)
        raise

    for path in fragment_files(draft / "fragments"):
        path.unlink()
    for name in CURATED:
        shutil.copyfile(TEMPLATE_DIR / name, draft / name)
    write_inventory_placeholder(draft / INVENTORY, "draft")
    if args.next:
        write_versions(draft, args.next, version)
    if in_tree:
        add_to_releases_index(series)

    print(f"froze Zephyr {version} into {output}")
    print(f"reset {draft}: fragments removed; highlights, security notice and inventory cleared")
    if not args.next:
        print(f"update {draft / 'version.rst.inc'} for the next release")
    if args.no_inventory:
        print(f"generate {output / 'change-inventory'} with {display_path(INVENTORY_SCRIPT)}")


def main(argv=None):
    parser = argparse.ArgumentParser(
        description="Manage release note fragments.", allow_abbrev=False
    )
    parser.add_argument("--draft", type=Path, default=DRAFT_DIR, help="draft directory")
    commands = parser.add_subparsers(dest="command", required=True)

    check = commands.add_parser("check", help="validate fragments")
    check.add_argument("dirs", nargs="*", type=Path)
    check.set_defaults(func=cmd_check)

    render_cmd = commands.add_parser("render", help="print the generated draft documents")
    render_cmd.add_argument("--document", action="append", choices=list(DOCUMENTS))
    render_cmd.set_defaults(func=cmd_render)

    inventory = commands.add_parser(
        "inventory", help="regenerate the change inventory of the draft"
    )
    inventory.add_argument("--start", help="start revision (default: v<last release>)")
    inventory.add_argument("--end", default=".", help="end revision (default: .)")
    inventory.set_defaults(func=cmd_inventory)

    freeze = commands.add_parser("freeze", help="write the draft into a release directory")
    freeze.add_argument("--next", type=version_arg, help="version of the following release")
    freeze.add_argument("--output", type=Path, help="release directory to create")
    freeze.add_argument(
        "--inventory-start", help="start revision of the change inventory (default: v<last>)"
    )
    freeze.add_argument(
        "--inventory-end", default=".", help="end revision of the change inventory (default: .)"
    )
    freeze.add_argument(
        "--no-inventory", action="store_true", help="do not generate the change inventory"
    )
    freeze.set_defaults(func=cmd_freeze)

    args = parser.parse_args(argv)
    try:
        return args.func(args)
    except (OSError, ValueError, subprocess.CalledProcessError) as err:
        sys.exit(str(err))


if __name__ == "__main__":
    sys.exit(main())
