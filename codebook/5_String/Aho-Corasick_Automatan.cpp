// Root 1; c-'A' in [0,sigma). Build O(top*sigma), scan O(|T|).
struct AC_Automatan {
  int nx[len][sigma], fl[len], cnt[len], ord[len], top;
  int rnx[len][sigma]; // complete transitions
  int newnode() {
    fill_n(nx[top], sigma, -1), fill_n(rnx[top], sigma, 0);
    fl[top] = cnt[top] = ord[top] = 0; return top++;
  }
  void init() { top = 1, newnode(); }
  int input(string &s) {
    int x = 1; for (char c : s) {
      int &v = nx[x][c - 'A']; if (v < 0) v = newnode();
      x = v;
    } return x; // pattern endpoint
  }
  void make_fl() {
    ord[0] = 1; for (int h = 0, t = 1; h < t; ++h) {
      int r = ord[h]; for (int i = 0; i < sigma; ++i) {
        int v = nx[r][i], f = r == 1 ? 1 : rnx[fl[r]][i];
        rnx[r][i] = v < 0 ? f : v;
        if (v >= 0) fl[v] = f, ord[t++] = v;
      }
    }
  }
  void solve() { // propagate visits once, after scanning
    for (int i = top - 2; i > 0; --i)
      cnt[fl[ord[i]]] += cnt[ord[i]];
  }
} ac;
