Matrix a; a.n = a.m = 2;
a.M[0][0] = 1 % P; a.M[0][1] = 2 % P;
a.M[1][0] = 3 % P; a.M[1][1] = 4 % P;
ll d = a.det(); // -2 modulo P; matrix a is overwritten
