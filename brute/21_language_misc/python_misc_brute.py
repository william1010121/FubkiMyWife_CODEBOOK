#!/usr/bin/env python3
"""codebook/11_Python/misc.py is a cheat sheet of snippets, not a module:
check that it parses and that the concrete results it states hold."""
from decimal import Context, Decimal, MAX_EMAX, MAX_PREC, ROUND_FLOOR
from decimal import localcontext
from fractions import Fraction
from pathlib import Path
import ast

TEMPLATE = Path(__file__).resolve().parents[2] / "codebook/11_Python/misc.py"
ast.parse(TEMPLATE.read_text(), str(TEMPLATE))
assert Fraction("3.14159").limit_denominator(10).numerator == 22
with localcontext(Context(prec=MAX_PREC, Emax=MAX_EMAX,
                          rounding=ROUND_FLOOR)):
    assert (Decimal("12345678901234567890") * Decimal("0.0000000001")
            == Decimal("1234567890.1234567890"))
assert pow(3, 200, 1000000007) == pow(3, 200) % 1000000007
print("python_misc_brute: PASS")
