#include "common.hpp"
namespace tested {
#include "../../codebook/6_Math/SchreierSims.cpp"
}
static vector<int> mul(const vector<int> &a, const vector<int> &b) { vector<int> r(a.size()); for (int i=0;i<(int)a.size();++i) r[i]=b[a[i]]; return r; }
static ll closure(const vector<vector<int>> &g, int n) {
  vector<int> id(n); iota(id.begin(), id.end(), 0); set<vector<int>> seen{ id }; queue<vector<int>> q; q.push(id);
  while (!q.empty()) { auto x=q.front(); q.pop(); for (auto &y:g) for (auto z:{mul(x,y),mul(y,x)}) if (seen.insert(z).second) q.push(z); }
  return seen.size();
}
int main() {
  mt19937 rng(1234);
  for (int tc=0;tc<150;++tc) {
    int n=1+rng()%6, k=rng()%4; vector<vector<int>> g;
    for(int z=0;z<k;++z){ vector<int> p(n); iota(p.begin(),p.end(),0); shuffle(p.begin(),p.end(),rng); g.pb(p); }
    tested::schreier::solve(g,n);
    require_ok(tested::schreier::size()==closure(g,n),"Schreier-Sims size");
    vector<int> id(n); iota(id.begin(),id.end(),0);
    require_ok(tested::schreier::inside(id),"Schreier-Sims identity");
  }
  cout << "schreier_sims: OK\n";
}
