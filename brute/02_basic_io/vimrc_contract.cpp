#include <bits/stdc++.h>

using namespace std;

[[noreturn]] void fail(const string &message) {
  cerr << "FAIL vimrc: " << message << '\n';
  exit(1);
}

void require(const string &text, const string &needle) {
  if (text.find(needle) == string::npos) fail("missing: " + needle);
}

int main() {
  ifstream file("codebook/1_Basic/vimrc.cpp");
  if (!file) fail("cannot open codebook/1_Basic/vimrc.cpp");
  string config((istreambuf_iterator<char>(file)), {});

  require(config, "setxkbmap -option ctrl:nocaps");
  require(config, "se nu ai hls et ru ic is sc cul re=");
  require(config, "1 ts=4 sts=4 sw=4 ls=2 mouse=a");
  require(config, "syntax on");
  require(config, "hi cursorline cterm=");
  require(config, "none ctermbg=89");
  require(config, "set bg=dark");
  require(config, "inoremap {<CR> {<CR>}<Esc>ko<tab>");
  require(config, "ca Hash w !cpp -dD -P -fpreprocessed \\|");
  require(config, "tr -d '[:space:]' \\| md5sum \\| cut -c-6");
  require(config, "ca Hash w !cpp -dD -P \\| sed");
  require(config, "ca Hash w !g++ -E - \\| sed");
  require(config, "cut -c-6");

  size_t mappings = 0;
  for (size_t pos = config.find("ca Hash"); pos != string::npos;
       pos = config.find("ca Hash", pos + 1))
    ++mappings;
  if (mappings != 3) fail("expected three Hash mappings");

  cerr << "PASS vimrc: keyboard, editor options, insert mapping, three Hash commands\n";
}
