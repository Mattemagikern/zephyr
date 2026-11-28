# SPDX-FileCopyrightText: The Zephyr Project Contributors
# SPDX-License-Identifier: Apache-2.0

import sys
from pathlib import Path

from west.commands import WestCommand

sys.path.append(str(Path(__file__).resolve().parents[1] / "release"))
import release_fragments


class ReleaseFragment(WestCommand):
    def __init__(self):
        super().__init__(
            "release-fragment",
            "create and check release note fragments",
            "Create, check and render the release note and migration guide fragments in "
            "doc/releases/draft/fragments.",
            accepts_unknown_args=True,
        )

    def do_add_parser(self, parser_adder):
        return parser_adder.add_parser(
            self.name, help=self.help, description=self.description, add_help=False
        )

    def do_run(self, args, unknown):
        status = release_fragments.main(unknown, prog="west release-fragment")
        if status:
            sys.exit(status)
