#include <cassert>
#include <fstream>
#include <string>
int main() {
  std::ifstream in("codebook/1_Basic/vimrc.cpp");
  assert(in);
  std::string all((std::istreambuf_iterator<char>(in)), {});
  assert(all.find("setxkbmap -option ctrl:nocaps") != std::string::npos);
  assert(all.find("syntax on") != std::string::npos);
  assert(all.find("ca Hash") != std::string::npos);
}
