vector<unsigned char> a(s.begin(), s.end()); // no 0 bytes
Suffix sf(a.data(), a.size());
sf.sa[i]; // i-th smallest suffix
sf.ra[i]; // rank of suffix i
sf.hi[i]; // lcp(sa[i], sa[i-1]); needs C++20
