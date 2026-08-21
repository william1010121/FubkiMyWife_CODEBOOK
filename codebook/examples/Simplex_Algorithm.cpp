int n=3, m=2; // max 3x+2y: x+y<=4, x<=2, y<=3
a[0][0]=1; a[0][1]=1; b[0]=4;
a[1][0]=1; b[1]=2; a[2][1]=1; b[2]=3;
c[0]=3; c[1]=2;
double value=simplex(n,m); // value=10, x={2,2}
