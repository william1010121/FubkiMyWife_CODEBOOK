#!/usr/bin/env python3
from __future__ import annotations

from decimal import Decimal
from fractions import Fraction
import importlib.util
from pathlib import Path
from random import Random


ROOT = Path(__file__).resolve().parents[2]
TEMPLATE = ROOT / "codebook/11_Python/misc.py"
SPEC = importlib.util.spec_from_file_location("codebook_python_misc", TEMPLATE)
assert SPEC is not None and SPEC.loader is not None
misc = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(misc)


def expect(condition: bool, message: str) -> None:
    if not condition:
        raise AssertionError(message)


def main() -> None:
    expect(
        misc.decimal_multiply("12345678901234567890", "0.0000000001")
        == Decimal("1234567890.1234567890"),
        "exact decimal multiplication",
    )
    expect(misc.decimal_multiply("-1.25", "-0.08") == Decimal("0.1000"),
           "signed decimal multiplication")
    expect(misc.limit_fraction("3.14159", 10) == Fraction(22, 7), "fraction limit")
    expect(misc.limit_fraction("-3.14159", 10) == Fraction(-22, 7),
           "negative fraction limit")
    expect(
        misc.read_matrix(["1 2 3", "4 5 6"], 2, 3) == [[1, 2, 3], [4, 5, 6]],
        "matrix parsing",
    )
    try:
        misc.read_matrix(["1 2"], 1, 3)
    except ValueError:
        pass
    else:
        raise AssertionError("matrix dimension validation")
    expect(misc.read_matrix([], 0, 0) == [], "empty matrix")
    expect(misc.read_matrix(["1 2", "3 4"], 1, 2) == [[1, 2]],
           "matrix parser ignores rows after n")

    for modulus in range(2, 50):
        for a in range(-20, 21):
            value = 1 % modulus
            for exponent in range(20):
                expect(
                    misc.modular_power(a, exponent, modulus) == value,
                    "modular power",
                )
                value = value * a % modulus
    expect(misc.modular_power(-3, 5, 7) == 2, "negative-base modular power")

    got = misc.random_examples(Random(12345), [1, 2, 3, 4])
    expect(got == misc.random_examples(Random(12345), [1, 2, 3, 4]), "seeded random")
    expect(sorted(got[2]) == [1, 2, 3, 4], "shuffle permutation")
    try:
        misc.random_examples(Random(1), [])
    except ValueError:
        pass
    else:
        raise AssertionError("empty random-example input")
    expect(
        misc.format_example(7, 3.14159, "ok") == "num: 7, pi: 3.14, str: ok",
        "formatted output",
    )
    expect(misc.parse_int_array(b"4 10 -2 8 9") == (4, [10, -2, 8, 9]), "fast input")
    expect(misc.parse_int_array(b"0") == (0, []), "empty fast input")
    for malformed in (b"", b"2 1", b"1 1 2", b"-1"):
        try:
            misc.parse_int_array(malformed)
        except ValueError:
            pass
        else:
            raise AssertionError("input length validation")

    print("python misc PASS")


if __name__ == "__main__":
    main()
