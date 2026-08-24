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


def _active_tex(content: str) -> str:
    """Remove TeX comments while retaining active lines and macro arguments."""
    active_lines = []
    for line in content.splitlines():
        chars = []
        backslashes = 0
        for char in line:
            if char == "%" and backslashes % 2 == 0:
                break
            chars.append(char)
            if char == "\\":
                backslashes += 1
            else:
                backslashes = 0
        active_lines.append("".join(chars))
    return "\n".join(active_lines)


def _braced_argument(text: str, start: int) -> tuple[str, int] | None:
    """Read one balanced TeX braced argument starting at *start*."""
    while start < len(text) and text[start].isspace():
        start += 1
    if start == len(text) or text[start] != "{":
        return None
    depth = 1
    index = start + 1
    while index < len(text) and depth:
        if text[index] == "\\":
            index += 2
            continue
        if text[index] == "{":
            depth += 1
        elif text[index] == "}":
            depth -= 1
        index += 1
    if depth:
        return None
    return text[start + 1 : index - 1], index


def _active_inputcodeex(text: str) -> list[tuple[str | None, str | None]]:
    """Return (source, example) arguments for active inputcodeex calls."""
    calls: list[tuple[str | None, str | None]] = []
    for match in re.finditer(r"\\inputcodeex\b", text):
        cursor = match.end()
        arguments: list[str] = []
        for _ in range(4):
            argument = _braced_argument(text, cursor)
            if argument is None:
                break
            value, cursor = argument
            arguments.append(value.strip())
        if len(arguments) == 4:
            calls.append((arguments[1], arguments[3]))
        else:
            source = arguments[1] if len(arguments) > 1 else None
            calls.append((source, None))
    return calls


def example_coverage() -> tuple[list[str], list[str], list[str], int]:
    """Validate examples attached to active inputcodeex calls only.

    Files left in ``codebook/examples`` for an unlisted or retired template are
    intentionally ignored.  Active calls still must have one existing example,
    and an example cannot be attached to multiple active calls.
    """
    content = _active_tex((CODEBOOK / "content.tex").read_text())
    calls = _active_inputcodeex(content)
    missing: list[str] = []
    found: list[str] = []
    for source, example in calls:
        if example is None:
            missing.append(source or "<inputcodeex>")
            continue
        if not re.fullmatch(r"examples/[^{}%\s]+\.cpp", example):
            missing.append(source or example)
            continue
        found.append(example)

    counts = Counter(found)
    available = {
        path.relative_to(CODEBOOK).as_posix()
        for path in (CODEBOOK / "examples").glob("*.cpp")
    }
    invalid = sorted(set(found) - available)
    duplicate = sorted(path for path, count in counts.items() if count != 1)
    return sorted(missing), invalid, duplicate, len(calls)


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
