from decimal import (Decimal, Context, localcontext, MAX_PREC, MAX_EMAX,
                     ROUND_FLOOR)
from fractions import Fraction
import sys
# Exact decimals; local context leaves the caller's context unchanged.
def decimal_multiply(a, b):
    with localcontext(Context(prec=MAX_PREC, Emax=MAX_EMAX,
                              rounding=ROUND_FLOOR)):
        return Decimal(a) * Decimal(b)
def limit_fraction(s, bound): return Fraction(s).limit_denominator(bound)
def read_matrix(lines, n, m):  # n,m>=0; at least n rows.
    it = iter(lines)
    a = [list(map(int, next(it).split())) for _ in range(n)]
    if any(len(row) != m for row in a): raise ValueError("row length")
    return a
def modular_power(a, b, m): return pow(a, b, m)
# rng = random.Random(seed); choices uses keyword k, uniform is lowercase.
def random_examples(rng, values):
    if not values: raise ValueError("empty choices")
    a = list(values); rng.shuffle(a)
    return rng.choice(values), rng.choices(values, k=2), a
# rng.randrange(L,R,step), rng.randint(L,R), rng.uniform(L,R)
def format_example(num, pi, text):
    return f"num: {num}, pi: {pi:.2f}, str: {text}"
def parse_int_array(data):  # data = sys.stdin.buffer.read()
    a = list(map(int, data.split()))
    if not a or a[0] < 0 or len(a) != a[0]+1: raise ValueError("length")
    return a[0], a[1:]
# with open("filename.in") as f: data = f.read()
# with open("filename.out", "w") as f: print(1, 2, sep=" | ", file=f)
# print("debug", file=sys.stderr); sys.stdin = open("filename.in")
# sys.set_int_max_str_digits(5000000)  # Python 3.11+, huge integer input
