#!/usr/bin/env python3
from __future__ import annotations

from decimal import Decimal, ROUND_CEILING, getcontext, localcontext
from fractions import Fraction
import importlib.util
import subprocess
import sys
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

    # Empty dimensions must not consume a row from an input iterator.
    rows = iter(["sentinel"])
    expect(misc.read_matrix(rows, 0, 8) == [] and next(rows) == "sentinel",
           "zero-row matrix preserves iterator")
    coefficient = "9" * 700
    expect(misc.decimal_multiply(coefficient, coefficient)
           == Decimal(int(coefficient) ** 2), "long exact coefficient")
    expect(misc.parse_int_array(b"  +3\r\n-0\t+1  -2\r\n")
           == (3, [0, 1, -2]), "stdin whitespace and signed zero")
    repeated = [1, 1, 2, 2]
    chosen, choices, shuffled = misc.random_examples(Random(8011), repeated)
    expect(repeated == [1, 1, 2, 2] and sorted(shuffled) == repeated
           and chosen in repeated and all(x in repeated for x in choices),
           "random duplicate values preserve multiplicity and input")
    # Closed stdin ensures the snippet stays safe to import.
    probe = "import sys, runpy\nsys.stdin.close()\nrunpy.run_path(sys.argv[1])"
    subprocess.run([sys.executable, "-c", probe, str(TEMPLATE)],
                   check=True, capture_output=True, text=True)
    rng = Random(1108)
    for _ in range(250):
        a, b = (rng.randrange(-10**80, 10**80) for _ in range(2))
        ea, eb = rng.randrange(-80, 81), rng.randrange(-80, 81)
        want = Decimal(f"{a*b}e{ea+eb}")
        expect(misc.decimal_multiply(f"{a}e{ea}", f"{b}e{eb}") == want,
               "exact signed coefficient/exponent oracle")
    # Enumerate alternatives to check the nearest bounded denominator.
    for p in range(-20, 21):
        for q in range(1, 10):
            target = Fraction(p, q)
            for bound in range(1, 11):
                got = misc.limit_fraction(f"{p}/{q}", bound)
                error = abs(got - target)
                expect(got.denominator <= bound, "fraction bound")
                for d in range(1, bound + 1):
                    num = (p*d) // q
                    expect(error <= abs(Fraction(num, d) - target)
                           and error <= abs(Fraction(num+1, d) - target),
                           "nearest fraction exhaustive oracle")
    # Exact float inputs are dyadic rationals, so build the expected
    # decimal from integer arithmetic without using Decimal multiplication.
    for _ in range(200):
        a, b = (rng.randrange(-2**20, 2**20) / 2**20 for _ in range(2))
        x, dx = a.as_integer_ratio()
        y, dy = b.as_integer_ratio()
        bits = (dx * dy).bit_length() - 1
        want = Decimal(f"{x*y*5**bits}e{-bits}")
        expect(misc.decimal_multiply(a, b) == want,
               "exact dyadic float product")
    expect(misc.decimal_multiply("1e-800000", "3e-800000")
           == Decimal("3e-1600000"), "exact subnormal decimal product")
    rows = iter([b"-1\t+2\r\n", b"3 4", b"sentinel"])
    expect(misc.read_matrix(rows, 2, 2) == [[-1, 2], [3, 4]]
           and next(rows) == b"sentinel", "byte-row iterator boundary")
    # Parsing/list ownership and byte tokenization at contest input sizes.
    values = [i % 199 - 99 for i in range(200000)]
    data = (str(len(values)) + "\n" + " ".join(map(str, values))).encode()
    n, parsed = misc.parse_int_array(data)
    expect(n == len(values) and parsed == values,
           "large signed integer input")
    parsed[0] = 123456
    expect(values[0] == -99 and misc.parse_int_array(data)[1] == values,
           "parse returns independently owned lists")
    source = [1, 1, 2]
    _, _, shuffled = misc.random_examples(Random(140006), source)
    shuffled.clear()
    expect(source == [1, 1, 2], "shuffle result has no source alias")
    print("python misc PASS")


if __name__ == "__main__":
    main()
