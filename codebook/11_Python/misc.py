from decimal import Context, Decimal, MAX_EMAX, MAX_PREC, ROUND_FLOOR, setcontext
from fractions import Fraction
from random import Random
import sys

if hasattr(sys, "set_int_max_str_digits"): sys.set_int_max_str_digits(5_000_000)

def decimal_multiply(a: str, b: str) -> Decimal:
    setcontext(Context(prec=MAX_PREC, Emax=MAX_EMAX, rounding=ROUND_FLOOR))
    return Decimal(a) * Decimal(b)

def limit_fraction(value: str, max_denominator: int) -> Fraction: return Fraction(value).limit_denominator(max_denominator)

def read_matrix(lines: list[str], n: int, m: int) -> list[list[int]]:
    matrix = [list(map(int, line.split())) for line in lines[:n]]
    if len(matrix) != n or any(len(row) != m for row in matrix):
        raise ValueError("matrix dimensions do not match n x m")
    return matrix

def modular_power(a: int, b: int, modulus: int) -> int: return pow(a, b, modulus)

def random_examples(rng: Random, values: list[int]) -> tuple[int, int, list[int]]:
    if not values:
        raise ValueError("values must be non-empty")
    shuffled = values.copy()
    rng.shuffle(shuffled)
    return rng.randint(1, 10), rng.choice(values), shuffled

def format_example(num: int, pi: float, text: str) -> str: return f"num: {num}, pi: {pi:.2f}, str: {text}"

def parse_int_array(data: bytes) -> tuple[int, list[int]]:
    tokens = data.split()
    if not tokens:
        raise ValueError("missing array length")
    n = int(tokens[0])
    if len(tokens) != n + 1:
        raise ValueError("array length does not match input")
    return n, list(map(int, tokens[1:]))
