#include "test1.h"
#include "tests_unittest.cpp"

#include <iostream>


int Fakoriyel(int n) {
  int result = 1;
  for (int i = 1; i <= n; i++) {
    result *= i;
  }
  return result;
}

bool AsalMi(int n) {
  if (n <= 1) return false;
  if (n % 2 == 0) return n == 2;

  for (int i = 3; ; i += 2) {
    if (i > n/i) break;

    if (n % i == 0) return false;
  }
  return true;
}


int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
