"""Exercise floor_enumeration.cpp with a real user body, not a copy."""
from pathlib import Path
import os
import subprocess
import tempfile

root = Path(__file__).resolve().parents[2]
source = (root / 'codebook/6_Math/floor_enumeration.cpp').read_text()
# Insert a body after the snippet computes x and r. Keep its actual loop.
body = r'''
    assert(l == next && l <= r && r <= n);
    assert(x == n/l && x == n/r);
    assert(l == 1 || n/(l-1) > x);
    assert(r == n || n/(r+1) < x);
    checksum += 1LL*(r-l+1)*x;
    next = r+1; ++blocks;
    x = -777; // User may reuse x; quotient descent must retain q.
'''
end = source.rfind('}')
assert end >= 0
instrumented = source[:end] + body + source[end:]
program = r'''
#include <bits/stdc++.h>
using namespace std;
void check(int n) {
  int next=1, blocks=0; long long checksum=0;
''' + instrumented + r'''
  assert(next == n+1);
  int s = sqrt(double(n));
  while (1LL*(s+1)*(s+1) <= n) ++s;
  while (1LL*s*s > n) --s;
  assert(blocks == 2*s-(n < 1LL*s*(s+1)));
  // Hyperbola identity gives an independent sum using a direct sqrt scan.
  long long sum=0;
  for (int i=1; i<=s; ++i) sum += n/i;
  assert(checksum == 2*sum-1LL*s*s);
}
int main() {
  for (int n=0; n<=10000; ++n) check(n);
  for (int k : {1,2,31,32,32767,46339,46340})
    for (int d : {-1,0,1}) check(k*k+d), check(k*(k+1)+d);
  for (int d=1; d<=256; ++d) check(INT_MAX-d);
  mt19937 rng(170006);
  for (int t=0; t<2000; ++t) check(rng()%(INT_MAX-1));
  cout << "PASS round17 floor: actual body, sums, mutation, boundaries\n";
}
'''
with tempfile.TemporaryDirectory(prefix='g06-floor-') as directory:
    path = Path(directory)
    (path / 'test.cpp').write_text(program)
    command = [os.environ.get('CXX', 'g++'), '-std=c++20', '-O2',
               '-fsanitize=undefined', '-fsanitize-undefined-trap-on-error',
               str(path / 'test.cpp'), '-o', str(path / 'test')]
    subprocess.run(command, cwd=root, check=True)
    subprocess.run([str(path / 'test')], cwd=root, check=True)
