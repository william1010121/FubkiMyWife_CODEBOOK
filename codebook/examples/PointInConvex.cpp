vector<IPoint> ch = {{0,0},{4,0},{4,4},{0,4}}; // CCW
int inside = pointInConvex(ch, {2,2}); // 2
int boundary = pointInConvex(ch, {4,2}); // 1
int outside = pointInConvex(ch, {5,2}); // 0
