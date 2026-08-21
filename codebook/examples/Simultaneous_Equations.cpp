matrix eq{}; eq.n = eq.m = 2;
eq.M[0][0]=1; eq.M[0][1]=1;  eq.M[0][2]=3;
eq.M[1][0]=2; eq.M[1][1]=-1; eq.M[1][2]=0;
int rank=eq.solve(); // sol={1,2}; -1: inconsistent
