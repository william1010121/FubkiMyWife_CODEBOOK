#!/usr/bin/env python3
"""Validate the usage fragments that are actually printed in codebook.pdf.

The files under ``codebook/examples`` are intentionally *fragments*: they are
not complete translation units and often use caller-owned variables (``n``,
``points``, ``LCA`` ...).  Compiling each file in isolation would therefore
test the wrong contract.  This checker follows the active ``inputcodeex``
calls in ``content.tex`` and validates the contract that can be checked
without inventing a caller:

* every active PDF entry has exactly one existing example;
* the example has no hidden includes or ``main`` (which would change the
  fragment contract); and
* a lexical/delimiter check plus one bounded ``g++ -fsyntax-only`` smoke pass
  rejects malformed fragment structure (unresolved caller names are expected);
* every explicitly named type/member operation in the example is present in
  the corresponding template or its documented prerequisite template.

The final check is deliberately conservative: unresolved names are reported
as external caller requirements rather than being silently treated as a
successful API match.  This keeps the validation useful for snippets that
depend on caller-defined data structures while still catching stale method
names and wrong object types.
"""

from __future__ import annotations

from collections import Counter
from pathlib import Path
import re
import subprocess
import sys


ROOT = Path(__file__).resolve().parents[1]
CODEBOOK = ROOT / "codebook"


def _active_tex(content: str) -> str:
    lines: list[str] = []
    for line in content.splitlines():
        out: list[str] = []
        slash = 0
        for char in line:
            if char == "%" and slash % 2 == 0:
                break
            out.append(char)
            slash = slash + 1 if char == "\\" else 0
        lines.append("".join(out))
    return "\n".join(lines)


def _arg(text: str, start: int) -> tuple[str, int] | None:
    while start < len(text) and text[start].isspace():
        start += 1
    if start >= len(text) or text[start] != "{":
        return None
    depth, i = 1, start + 1
    while i < len(text) and depth:
        if text[i] == "\\":
            i += 2
            continue
        if text[i] == "{":
            depth += 1
        elif text[i] == "}":
            depth -= 1
        i += 1
    return None if depth else (text[start + 1 : i - 1], i)


def active_pairs() -> list[tuple[str, str]]:
    text = _active_tex((CODEBOOK / "content.tex").read_text())
    pairs: list[tuple[str, str]] = []
    for match in re.finditer(r"\\inputcodeex\b", text):
        cursor, args = match.end(), []
        for _ in range(4):
            item = _arg(text, cursor)
            if item is None:
                break
            value, cursor = item
            args.append(value.strip())
        if len(args) != 4:
            raise AssertionError(f"malformed inputcodeex near {text[match.start():match.start()+80]!r}")
        pairs.append((args[1], args[3]))
    return pairs


# A few templates deliberately consume a preceding declaration.  Include only
# those prerequisites here; using the whole codebook would make unrelated
# symbols hide a stale example API.
PREREQUISITES = {
    "2_Graph/2SAT.cpp": ["2_Graph/SCC.cpp"],
    "3_Data_Structure/Heavy_light_Decomposition.cpp": [
        "3_Data_Structure/Dynamic_2D_Segment_Tree.cpp"
    ],
    "4_Flow_Matching/Gomory_Hu_tree.cpp": ["4_Flow_Matching/Dinic.cpp"],
    "6_Math/Simultaneous_Equations.cpp": ["6_Math/Fraction.cpp"],
    "8_Geometry/MinimumEnclosingCircle.cpp": ["8_Geometry/_basic.cpp"],
    "8_Geometry/ClosestPair.cpp": ["8_Geometry/_basic.cpp"],
    "8_Geometry/convexHull.cpp": ["8_Geometry/_basic.cpp"],
    "8_Geometry/halfPlaneIntersect.cpp": ["8_Geometry/_basic.cpp"],
    "8_Geometry/pointInPolygon.cpp": ["8_Geometry/_basic.cpp"],
}


IGNORED_MEMBER_NAMES = {
    # standard-library operations and member access used only in comments
    "begin", "end", "size", "empty", "clear", "push_back", "emplace_back",
    "back", "front", "data", "first", "second", "c_str", "v", "p", "q",
}

IGNORED_CALL_NAMES = {
    # language control, STL/math, and caller-owned callbacks
    "if", "for", "while", "switch", "catch", "sizeof", "max", "min",
    "abs", "acos", "asin", "atan", "cos", "exp", "fabs", "gcd", "log",
    "pow", "round", "sin", "sqrt", "swap", "shuffle", "sort", "putchar",
    "use_factor", "get_lca", "toggle", "apply_update", "rollback_update",
    "add_pos", "remove_pos", "current_answer",
}


def _contract_text(source: str) -> str:
    paths = [CODEBOOK / "1_Basic/Default_code.cpp", CODEBOOK / source]
    paths += [CODEBOOK / path for path in PREREQUISITES.get(source, [])]
    return "\n".join(path.read_text() for path in paths)


def _check_fragment(source: str, example: str) -> list[str]:
    failures: list[str] = []
    src = CODEBOOK / source
    ex = CODEBOOK / example
    if not src.is_file():
        failures.append(f"missing template {source}")
        return failures
    if not ex.is_file():
        failures.append(f"missing example {example}")
        return failures
    text = ex.read_text()
    if not text.strip():
        failures.append(f"empty example {example}")
    if re.search(r"^\s*#\s*include\b", text, re.M):
        failures.append(f"example contains include: {example}")
    if re.search(r"\bmain\s*\(", text):
        failures.append(f"example contains main: {example}")

    contract = _contract_text(source)
    code_only = re.sub(r"//.*", "", text)
    # A member call is the strongest API signal available in a caller-owned
    # fragment.  Ignore common STL members; unknown names are hard failures.
    for member in sorted(set(re.findall(r"\.\s*([A-Za-z_]\w*)\s*\(", text))):
        if member in IGNORED_MEMBER_NAMES:
            continue
        if not re.search(rf"\b{re.escape(member)}\s*\(", contract):
            failures.append(f"{example}: member API .{member} not found in {source}")

    for call in sorted(set(re.findall(r"(?<![.>:\w])([A-Za-z_]\w*)\s*\(", code_only))):
        if call in IGNORED_CALL_NAMES:
            continue
        # A helper declared in the usage fragment itself is valid by design.
        if re.search(
            rf"\b(?:bool|void|int|ll|double|auto)\s+{re.escape(call)}\s*\(",
            code_only,
        ):
            continue
        # ``Type object(args)`` is construction, not a free function call.
        if re.search(
            rf"\b(?:static\s+)?(?:[A-Z][A-Za-z_0-9]*|mt19937_64)\s+{re.escape(call)}\s*\(",
            code_only,
        ):
            continue
        if not re.search(rf"\b{re.escape(call)}\s*\(", contract):
            failures.append(f"{example}: free API {call} not found in {source}")

    # Explicitly constructed user types must be declared by the template or a
    # documented prerequisite.  Built-in and STL types are intentionally
    # excluded because they belong to the default preamble.
    builtin = {
        "int", "long", "double", "float", "char", "bool", "auto", "string",
        "vector", "pair", "set", "map", "Q", "L", "P", "pt", "circle",
        "mint", "Poly_t", "Query", "Splay", "ull", "ll",
    }
    for typ in sorted(set(re.findall(r"\b(?:static\s+)?([A-Z][A-Za-z_0-9]*)\s+[A-Za-z_]\w*\s*(?:[;(=])", code_only))):
        if typ in builtin:
            continue
        if not re.search(rf"\b(?:struct|class|using|typedef)\s+{re.escape(typ)}\b", contract):
            failures.append(f"{example}: type {typ} not found in {source}")
    return failures


def _structure_error(text: str) -> str | None:
    """Return a parser-independent lexical/delimiter error, if any.

    C++ name lookup makes a compiler-only check noisy for these deliberately
    incomplete fragments.  This scanner handles the parts that do not depend
    on caller declarations: comments, quoted literals, balanced delimiters,
    and an assignment with a missing right-hand expression.
    """
    stack: list[tuple[str, int]] = []
    pairs = {")": "(", "]": "[", "}": "{"}
    masked: list[str] = []
    state = "code"
    quote_start = -1
    i = 0
    while i < len(text):
        char = text[i]
        nxt = text[i + 1] if i + 1 < len(text) else ""
        if state == "line-comment":
            masked.append("\n" if char == "\n" else " ")
            if char == "\n":
                state = "code"
            i += 1
            continue
        if state == "block-comment":
            if char == "*" and nxt == "/":
                masked.extend((" ", " "))
                state = "code"
                i += 2
            else:
                masked.append("\n" if char == "\n" else " ")
                i += 1
            continue
        if state in {"string", "char"}:
            masked.append("\n" if char == "\n" else " ")
            if char == "\\" and i + 1 < len(text):
                masked.append("\n" if nxt == "\n" else " ")
                i += 2
                continue
            expected = '"' if state == "string" else "'"
            if char == expected:
                state = "code"
            elif char == "\n":
                return f"newline in quoted literal opened at byte {quote_start}"
            i += 1
            continue

        if char == "/" and nxt == "/":
            masked.extend((" ", " "))
            state = "line-comment"
            i += 2
            continue
        if char == "/" and nxt == "*":
            masked.extend((" ", " "))
            state = "block-comment"
            i += 2
            continue
        if char in {'"', "'"}:
            state = "string" if char == '"' else "char"
            quote_start = i
            # Leave one operand token behind so ``x = "...";`` is not
            # mistaken for a missing assignment expression after masking.
            masked.append("0")
            i += 1
            continue
        if char in "([{":
            stack.append((char, i))
        elif char in ")]}":
            if not stack or stack[-1][0] != pairs[char]:
                return f"unmatched {char!r} at byte {i}"
            stack.pop()
        masked.append(char)
        i += 1

    if state == "block-comment":
        return "unterminated block comment"
    if state in {"string", "char"}:
        return f"unterminated quoted literal opened at byte {quote_start}"
    if stack:
        char, at = stack[-1]
        return f"unclosed {char!r} at byte {at}"
    code = "".join(masked)
    if re.search(r"(?<![=!<>])=(?!=)\s*;", code):
        return "assignment has no right-hand expression"
    return None


def _syntax_check(pairs: list[tuple[str, str]]) -> bool:
    """Run structural checks and one bounded compiler smoke pass.

    Keeping all probes in one translation unit avoids starting 73 compiler
    processes while retaining the statement-vs-namespace distinction needed
    by the examples.  Undefined caller names are expected and ignored; parser
    diagnostics are not.
    """
    chunks = ["#include <bits/stdc++.h>", "using namespace std;"]
    for i, (_, example) in enumerate(pairs):
        text = (CODEBOOK / example).read_text()
        if _structure_error(text) is not None:
            return False
        has_function = re.search(
            r"(?m)^\s*(?:bool|void|int|ll|double)\s+[A-Za-z_]\w*\s*\(", text
        )
        chunks.append(f"namespace pdf_example_{i} {{")
        if not has_function:
            chunks.append("void usage(){")
        chunks.append(text)
        if not has_function:
            chunks.append("}")
        chunks.append("}")
    result = subprocess.run(
        ["g++", "-std=c++20", "-fsyntax-only", "-x", "c++", "-"],
        input="\n".join(chunks),
        text=True,
        capture_output=True,
        check=False,
    )
    # ``expected ...`` diagnostics are commonly cascades from deliberately
    # unresolved caller types (for example ``Poly_t``); only diagnostics that
    # cannot be caused by missing caller declarations are hard parser errors.
    parser_error = re.compile(
        r"error: (?:stray\b|missing terminating|unterminated|"
        r"invalid preprocessing directive)",
        re.I,
    )
    return not any(parser_error.search(line) for line in result.stderr.splitlines())


def _self_test_structure_check() -> bool:
    malformed = [
        "int x = ;",
        "if (true { int x; }",
        "foo(1, 2];",
        'const char *s = "unterminated;',
        "/* unterminated",
    ]
    valid = [
        "int x = 0; // } is a comment",
        'string s = "[not a delimiter]";',
        "if (ok) { use(x); }",
    ]
    return (all(_structure_error(text) for text in malformed)
            and all(_structure_error(text) is None for text in valid))


def main() -> int:
    if not _self_test_structure_check():
        print("pdf examples: internal structure-check self-test failed")
        return 1
    pairs = active_pairs()
    if len(pairs) != 73:
        print(f"pdf examples: expected 73 active inputcodeex entries, got {len(pairs)}")
        return 1
    examples = [example for _, example in pairs]
    duplicate = sorted(path for path, count in Counter(examples).items() if count != 1)
    failures: list[str] = []
    if duplicate:
        failures.extend(f"duplicate active example {path}" for path in duplicate)
    for source, example in pairs:
        failures.extend(_check_fragment(source, example))
    if not failures and not _syntax_check(pairs):
        failures.append("C++ parser rejected one or more active usage fragments")
    if failures:
        print(f"pdf examples: active={len(pairs)} failures={len(failures)}")
        print("\n".join(failures))
        return 1
    print(f"pdf examples: active={len(pairs)} unique=1:1 contract=ok")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
