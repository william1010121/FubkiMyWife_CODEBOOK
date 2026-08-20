ac.init();
int end = ac.input(pat); ac.cnt[end]++;
ac.make_fl(); // build fail + rnx
for (char c : text) x = ac.rnx[x][c - 'A'];
ac.solve(); // push cnt up the fail tree
