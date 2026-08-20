ex.init();
for (auto &s : strs) ex.insert(s); // trie
ex.build(); // trie -> generalised SAM
ex.solve(); // cnt = occurrences per state
