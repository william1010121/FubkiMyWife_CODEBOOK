from decimal import Decimal as D, getcontext
getcontext().prec = 100
pi = D('3.14159265358979323846264338327950288419716939937510'
       '58209749445923078164062862089986280348253421170679')
def atan(x):
    if x < 0: return -atan(-x)
    if x > 1: return pi / 2 - atan(1 / x)
    factor = 1
    while x > D('.1'):
        x = x / (1 + (1 + x*x).sqrt())
        factor *= 2
    term = total = x
    for k in range(1, 100):
        term *= -x*x
        total += term / (2*k + 1)
    return factor * total
values = []
for r in [D('1e4'),D('1e6'),D('1e9')]:
    for shift in [D('-.5'),D(0),D('.5')]:
        d = r + shift
        x = (1 + d*d - r*r) / (2*d)
        y = (1 - x*x).sqrt()
        p = atan(y/x) + (pi if x < 0 else 0) if x else pi/2
        q = atan(y/(d-x))
        want = p + r*r*q - d*y
        values.append((r,shift,want))

# Verify the independently stored circle.cpp regression constants.
from pathlib import Path
import re
source = Path(__file__).with_name('round5_geometry_brute.cpp').read_text()
block = source.split('vector<array<double, 3>> cases = {')[1]
block = block.split('};')[0]
cases = re.findall(r'\{([^,]+),([^,]+),([^}]+)\}', block)
assert len(cases) == len(values)
for row, expected in zip(cases, values):
    got = tuple(D(x.strip()) for x in row)
    assert got[:2] == expected[:2]
    assert abs(got[2] - expected[2]) < D('1e-18')
print('round5 100-digit lens oracle constants PASS')
