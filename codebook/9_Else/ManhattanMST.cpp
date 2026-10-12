// <=4n edges {w,i,j} containing a Manhattan MST.
vector<array<ll, 3>> manhattan(vector<pll> p) {
  vector<int> id(SZ(p)); iota(ALL(id), 0);
  vector<array<ll, 3>> es;
  for (int k = 0; k < 4; ++k) {
    sort(ALL(id), [&](int i, int j) {
      return p[i].X - p[j].X < p[j].Y - p[i].Y; });
    map<ll, int> sw; // -y -> id
    for (int i : id) {
      for (auto it = sw.lower_bound(-p[i].Y);
           it != sw.end(); sw.erase(it++)) {
        int j = it->Y;
        ll dx = p[i].X - p[j].X, dy = p[i].Y - p[j].Y;
        if (dy > dx) break; es.pb({dx + dy, i, j});
      }
      sw[-p[i].Y] = i;
    }
    for (auto &q : p) if (k & 1) q.X = -q.X;
                      else swap(q.X, q.Y);
  }
  return es; }
