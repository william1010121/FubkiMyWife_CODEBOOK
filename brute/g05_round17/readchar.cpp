#include "../../codebook/1_Basic/Default_code.cpp"
#include <sys/wait.h>
#include <unistd.h>
#ifdef __APPLE__
#define fread_unlocked fread
#endif
#include "../../codebook/1_Basic/readchar.cpp"

// Each fork gets a fresh reader state and exactly one stdin stream.
// The parent never calls readchar, including between test cases.
int main() {
  for (int n : {0,1,2,255,256,257,65534,65535,65536,65537,
                65538,131071,131072,131073,1048577}) {
    pid_t child = fork(); assert(child >= 0);
    if (!child) {
      FILE *input = tmpfile(); assert(input);
      vector<unsigned char> bytes(n);
      for (int i = 0; i < n; ++i) bytes[i] = (i*131+255)&255;
      if (n) bytes.back() = 255;
      assert(fwrite(bytes.data(),1,n,input) == size_t(n));
      assert(fflush(input) == 0); rewind(input);
      assert(dup2(fileno(input),STDIN_FILENO) >= 0);
      clearerr(stdin);
      for (int x : bytes) assert(readchar() == x);
      for (int repeat = 0; repeat < 100; ++repeat)
        assert(readchar() == EOF);
      fclose(input); _exit(0);
    }
    int status; assert(waitpid(child,&status,0) == child);
    assert(WIFEXITED(status) && WEXITSTATUS(status) == 0);
  }
  cout << "PASS round17 readchar: 15 fresh streams/refill/byte/EOF\n";
}
