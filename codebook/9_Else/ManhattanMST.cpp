void solve(Point *a, int n) {
    sort(a, a + n, [](const Point &p, const Point &q) {
        return p.x + p.y < q.x + q.y;
    });
    map<long long, int> sweep; // key = -y
    for (int i = 0; i < n; ++i) {
        for (auto it = sweep.lower_bound(-a[i].y); it != sweep.end(); it = sweep.erase(it)) {
            int j = it->second;
            if (a[i].x - a[j].x < a[i].y - a[j].y) break;
            es.push_back({a[j].u, a[i].u, dist(a[j], a[i])});
        }
        sweep[-a[i].y] = i;
    }
}
void MST(Point *a, int n) {
    for (int t = 0; t < 2; ++t) {
        solve(a, n);
        for (int i = 0; i < n; ++i) swap(a[i].x, a[i].y);
        solve(a, n);
        for (int i = 0; i < n; ++i) a[i].x = -a[i].x;
    }
}
