// b!=0; evaluated ll arithmetic (incl. rounding) must fit.
complex<ll> gaussian_gcd(complex<ll> a, complex<ll> b) {
  ll c = a.real()*b.real() + a.imag()*b.imag(),
     d = a.imag()*b.real() - a.real()*b.imag(),
     r = b.real()*b.real() + b.imag()*b.imag();
  auto rnd = [&](ll x) { return (2*x + (x >= 0 ? r : -r))/(2*r); };
  if (c % r == 0 && d % r == 0) return b;
  return gaussian_gcd(b, a - complex<ll>(rnd(c), rnd(d))*b);
}
