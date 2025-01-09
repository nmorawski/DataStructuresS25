#include <iostream>
#include <cstdio>

int main() {
  int count = 0;
  while (true) {
    count++;
    if (count > 1000000) {
      // just waste alot of time!
      count = 0;
      std::cout << ".";
      fflush(stdout);
    }
  }
}
