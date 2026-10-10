template<class Func, class d = double>
struct Simpson {
  using pdd = pair<d, d>; Func f;
  pdd mix(pdd l, pdd r, optional<d> fm = {}) { // {x,f(x)}
    d h = (r.first - l.first) / 2;
    d v = fm ? *fm : f(l.first + h);
    return {v, h / 3 * (l.second + 4 * v + r.second)};
  }
  d eval(pdd l, pdd r, d fm, d eps) { // fm=f(midpoint)
    pdd m(l.first + (r.first - l.first) / 2, fm);
    d s = mix(l, r, fm).second;
    auto [flm, sl] = mix(l, m);
    auto [fmr, sr] = mix(m, r);
    d delta = sl + sr - s;
    if (abs(delta) <= 15 * eps)
      return sl + sr + delta / 15;
    return eval(l, m, flm, eps / 2)
         + eval(m, r, fmr, eps / 2);
  }
  // Attainable eps>0; smooth deterministic f; finite math.
  // Enough stack; error estimate, not a rigorous bound.
  d eval(d l, d r, d eps) {
    return eval({l, f(l)}, {r, f(r)},
                f(l + (r - l) / 2), eps);
  }
  // k>0; pre-split peaks, reuse endpoints.
  d eval2(d l, d r, d eps, int k = 997) {
    d h = (r - l) / k, s = 0; pdd a(l, f(l));
    for (int i = 0; i < k; ++i) {
      d x = i+1 == k ? r : l + h*(i+1); pdd b(x, f(x));
      s += eval(a, b, f(a.first + (x-a.first)/2), eps/k);
      a = b;
    } return s;
  }
};
template<class Func> Simpson<Func> make_simpson(Func f) {
  return {f}; }
