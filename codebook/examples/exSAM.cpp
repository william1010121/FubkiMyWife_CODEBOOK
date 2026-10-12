static exSAM ex; ex.init();
for (auto &s : strs) ex.insert(s); // trie
ex.build(); ex.solve(); // cnt = occurrences per state
