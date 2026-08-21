#!/usr/bin/env python3
"""Check that every codebook artifact is accounted for by the brute suite.

This is intentionally a structural audit.  Algorithm correctness remains the
job of the C++ oracle harnesses; this script prevents a newly added template
from silently missing those harnesses.
"""

from __future__ import annotations

from collections import Counter
from pathlib import Path
import re
import sys


ROOT = Path(__file__).resolve().parents[1]
CODEBOOK = ROOT / "codebook"
BRUTE = ROOT / "brute"
SEARCHABLE_SUFFIXES = {
    ".cpp",
    ".hpp",
    ".json",
    ".md",
    ".py",
    ".sh",
    ".tsv",
}
SKIP_DIRS = {"examples", "codebook_cp"}
SKIP_NAMES = {"tmp"}
SOURCE_SUFFIXES = {".cpp", ".py", ".sh"}
SOURCE_WITHOUT_SUFFIX = {"Number_Theory_Transform_Prime"}


def codebook_artifacts() -> list[Path]:
    return [
        path
        for path in sorted(CODEBOOK.glob("*/*"))
        if path.is_file()
        and path.parent.name not in SKIP_DIRS
        and path.name not in SKIP_NAMES
        and (path.suffix in SOURCE_SUFFIXES or path.name in SOURCE_WITHOUT_SUFFIX)
    ]


def active_callable_artifacts() -> list[Path]:
    """Return source listings enabled in content.tex (ignore commented lines)."""
    content = (CODEBOOK / "content.tex").read_text()
    active_text = "\n".join(
        line for line in content.splitlines() if not line.lstrip().startswith("%")
    )
    paths = set()
    for match in re.finditer(r"(?<![A-Za-z0-9_.-])([0-9]+_[^{}%\s]+)", active_text):
        path = CODEBOOK / match.group(1)
        if path.is_file() and (
            path.suffix in SOURCE_SUFFIXES or path.name in SOURCE_WITHOUT_SUFFIX
        ):
            paths.add(path)
    return sorted(paths)


def example_coverage() -> tuple[list[str], list[str], list[str], int]:
    """Return missing, extra, and duplicate active example snippet paths."""
    content = (CODEBOOK / "content.tex").read_text()
    active_text = "\n".join(
        line for line in content.splitlines() if not line.lstrip().startswith("%")
    )
    found = re.findall(r"examples/[^}%\s]+\.cpp", active_text)
    counts = Counter(found)
    expected = {
        path.relative_to(CODEBOOK).as_posix()
        for path in (CODEBOOK / "examples").glob("*.cpp")
    }
    got = set(found)
    return (
        sorted(expected - got),
        sorted(got - expected),
        sorted(path for path, count in counts.items() if count != 1),
        len(expected),
    )


def searchable_brute_files() -> list[tuple[Path, str]]:
    result: list[tuple[Path, str]] = []
    for path in sorted(BRUTE.rglob("*")):
        if not path.is_file() or path.suffix not in SEARCHABLE_SUFFIXES:
            continue
        try:
            result.append((path, path.read_text(errors="ignore")))
        except OSError as error:
            print(f"cannot read {path.relative_to(ROOT)}: {error}", file=sys.stderr)
            raise
    return result


def main() -> int:
    brute_files = searchable_brute_files()
    executable_harnesses = [
        (path, text)
        for path, text in brute_files
        if path.suffix == ".cpp" or path.name.endswith("_brute.py")
    ]
    all_sources = codebook_artifacts()
    active_sources = active_callable_artifacts()
    missing: list[Path] = []
    for template in active_sources:
        relative = template.relative_to(ROOT).as_posix()
        if not any(relative in text for _, text in executable_harnesses):
            missing.append(template)

    referenced = []
    executable_referenced = []
    for template in all_sources:
        relative = template.relative_to(ROOT).as_posix()
        if any(relative in text for _, text in brute_files):
            referenced.append(template)
        if any(relative in text for _, text in executable_harnesses):
            executable_referenced.append(template)

    if "--all" in sys.argv[1:]:
        executable_set = set(executable_referenced)
        for template in all_sources:
            if template not in executable_set:
                print(f"NO-EXEC\t{template.relative_to(ROOT).as_posix()}")
    executable_set = set(executable_referenced)
    missing_all = [template for template in all_sources if template not in executable_set]
    example_missing, example_extra, example_duplicate, example_count = example_coverage()

    if missing or missing_all or example_missing or example_extra or example_duplicate:
        print(
            "coverage audit: "
            f"active={len(active_sources)} active-missing={len(missing)} "
            f"all-sources={len(all_sources)} accounted={len(referenced)} "
            f"executable={len(executable_referenced)} all-missing={len(missing_all)} "
            f"example-missing={len(example_missing)} example-extra={len(example_extra)} "
            f"example-duplicate={len(example_duplicate)}"
        )
        for path in missing:
            print(path.relative_to(ROOT).as_posix())
        for label, paths in (
            ("EXAMPLE-MISSING", example_missing),
            ("EXAMPLE-EXTRA", example_extra),
            ("EXAMPLE-DUPLICATE", example_duplicate),
        ):
            for path in paths:
                print(f"{label}\t{path}")
        return 1

    print(
        "coverage audit: "
        f"active={len(active_sources)} active-missing=0 "
        f"all-sources={len(all_sources)} accounted={len(referenced)} "
        f"executable={len(executable_referenced)} all-missing=0 "
        f"examples={example_count}/{example_count}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
