struct fraction { // Reduced rationals; intermediates must fit ll.
  ll n, d;
  fraction(const ll &a=0, const ll &b=1): n(a), d(b) {
    ll g = gcd(n, d); n /= g, d /= g;
    if (d < 0) n = -n, d = -d;
  }
  fraction operator-() const { return fraction(-n, d); }
  fraction operator+(const fraction &b) const {
    return fraction(n*b.d + b.n*d, d*b.d); }
  fraction operator-(const fraction &b) const {
    return fraction(n*b.d - b.n*d, d*b.d); }
  fraction operator*(const fraction &b) const {
    return fraction(n*b.n, d*b.d); }
  fraction operator/(const fraction &b) const {
    return fraction(n*b.d, d*b.n); }
  void print() { cout << n; if (d != 1) cout << "/" << d; }
};
