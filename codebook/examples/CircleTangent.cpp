C a({0,0},1), b({4,0},1); // positive radii
vector<L> lines = tangent(a,b); // four common tangents
vector<L> fromPoint = tangent(a,P(2,0)); // two tangents
vector<L> coincident = tangent(a,a); // empty finite-list convention
