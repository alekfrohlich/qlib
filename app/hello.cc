#include <std/display.h>
#include <std/ostream.h>

void main(void) {
  using namespace std;
  for (int i = 0; i < 26; i++) {
    cout << i << std::endl;
  }
  display.scroll();
}