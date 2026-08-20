Suffix sf(s.c_str(), s.size()); // no 0 bytes
sf.sa[i]; // i-th smallest suffix
sf.ra[i]; // rank of suffix i
sf.hi[i]; // lcp(sa[i], sa[i-1]); needs C++20
