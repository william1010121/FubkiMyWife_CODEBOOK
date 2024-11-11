from decimal import *
setcontext(Context(prec=MAX_PREC, Emax=MAX_EMAX, rounding=ROUND_FLOOR))
print(Decimal(input()) * Decimal(input()))
from fractions import Fraction
Fraction('3.14159').limit_denominator(10).numerator # 22

map(int,input().split())
# N*M
arr2d = [ [ list(map(int,input().split())) ] for i in range(N) ]
" ".join(str(i) for i in a)
#a^b%M
pow(a,b,M) 
