# decimal
from decimal import *

setcontext(Context(prec=MAX_PREC, Emax=MAX_EMAX, rounding=ROUND_FLOOR))
print(Decimal(input()) * Decimal(input()))
from fractions import Fraction

Fraction("3.14159").limit_denominator(10).numerator  # 22
# N*M
map(int, input().split())
arr2d = [[list(map(int, input().split()))] for i in range(N)]
# a^b%M
pow(a, b, M)
# random
from random import *

randrange(L, R, step)  # [L,R) L+k*step
randint(L, R)  # int from [L,R]
choice(list)  # pick 1 item from list
choices(list, k)  # pick k item
shuffle(list)
Uniform(L, R)  # float from [L,R]
# print
print(f"num: {num}, pi: {pi:.2f}, str: {text}")
print(1, 2, 3, 4, sep=" | ", end=" <-- END\n")
# file IO
r = open("filename.in")
a = r.read()  # read whole content into one string
w = open("filename.out", "w")
w.write("123\n")
# IO redirection
import sys

sys.stdin = open("filename.in")
sys.stdout = open("filename.out", "w")
print("123", file=sys.stderr)


input_data = sys.stdin.buffer.read().split()
n = int(input_data[0])
arr = list(map(int, input_data[1 : n + 1]))


sys.set_int_max_str_digits(5000000)
