#!/usr/bin/env python3
from __future__ import annotations

from decimal import Decimal, ROUND_CEILING, getcontext, localcontext
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
    with localcontext() as ctx:
        ctx.prec, ctx.Emin, ctx.Emax = 2, -2, 2
        ctx.rounding = ROUND_CEILING
        before = ctx.copy()
        expect(misc.decimal_multiply("1.23456789e-5000", "1e-5000")
               == Decimal("1.23456789e-10000"), "tiny exact decimals")
        expect(misc.decimal_multiply("9e5000", "9e5000")
               == Decimal("81e10000"), "large exact decimals")
        after = getcontext()
        for field in ("prec", "Emin", "Emax", "rounding",
                      "traps", "flags"):
            expect(getattr(after, field) == getattr(before, field),
                   "decimal caller context changed")
    expect(misc.decimal_multiply("-0", "3").is_signed(), "signed zero")
    expect(misc.limit_fraction("3.14159", 10) == Fraction(22, 7), "fraction limit")
    expect(misc.limit_fraction("-3.14159", 10) == Fraction(-22, 7),
           "negative fraction limit")
    expect(misc.limit_fraction("1/2", 1) == Fraction(0), "fraction tie")
    expect(misc.limit_fraction("1/3", 10**100) == Fraction(1, 3),
           "large fraction bound")
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
    expect(misc.read_matrix(["", "  "], 2, 0) == [[], []],
           "zero-column matrix")
    rows = iter(["-1 +2", "3 4", "unused"])
    expect(misc.read_matrix(rows, 2, 2) == [[-1, 2], [3, 4]]
           and next(rows) == "unused", "matrix iterator consumption")
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
    expect(misc.modular_power(3, -1, 11) == 4, "modular inverse")
    expect(misc.modular_power(123, 0, 1) == 0, "unit modulus")
    expect(misc.modular_power(3, 2, -7) == -5, "negative modulus")
    for args in ((1, 1, 0), (2, -1, 4)):
        try:
            misc.modular_power(*args)
        except ValueError:
            pass
        else:
            raise AssertionError("invalid modular power accepted")

    got = misc.random_examples(Random(12345), [1, 2, 3, 4])
    expect(got == misc.random_examples(Random(12345), [1, 2, 3, 4]), "seeded random")
    expect(sorted(got[2]) == [1, 2, 3, 4], "shuffle permutation")
    value = 10**100
    values = (value,)
    expect(misc.random_examples(Random(1), values) ==
           (value, [value, value], [value]) and values == (value,),
           "singleton random sequence preserved")
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
    huge = 10**600
    expect(misc.parse_int_array(f"+2\n {huge}\t-{huge}\n".encode())
           == (2, [huge, -huge]), "large signed integer parsing")
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
