// b!=0; the quotient must fit int (exclude INT_MIN/-1).
int floor(int a, int b) { return a/b - (a%b && (a<0) != (b<0)); }
int ceil(int a, int b) { return a/b + (a%b && (a<0) == (b<0)); }
