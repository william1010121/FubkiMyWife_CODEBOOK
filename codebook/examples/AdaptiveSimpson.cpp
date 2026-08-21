auto f=make_simpson([](double x){return sin(x);});
double area=f.eval(0,acos(-1.0),1e-10); // area ~= 2
