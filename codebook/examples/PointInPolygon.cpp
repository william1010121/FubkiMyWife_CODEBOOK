vector<P> ps = {{0,0},{4,0},{4,4},{2,1},{0,4}}; // concave
int inside = pip(ps, {1,1}); // 2
int boundary = pip(ps, {3,2.5}); // 1
int notch = pip(ps, {2,3}); // 0
