Point a[]={{0,0},{1,0},{1,1},{0,1}};
Point b[]={{3,0},{4,0},{4,1},{3,1}};
double d=TwoConvexHullMinDist(a,b,4,4); // 2; O(n+m)
// Nonempty convex polygons; either cyclic orientation is accepted.
