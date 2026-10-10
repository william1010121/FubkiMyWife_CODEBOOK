ac.init(); vector<int> ends;
for (auto &pat : pats) ends.pb(ac.input(pat));
ac.make_fl(); int x = 1;
for (char c : text)
  x = ac.rnx[x][c - 'A'], ++ac.cnt[x];
ac.solve(); // propagate visits once
ac.cnt[ends[i]]; // occurrences of pats[i]
