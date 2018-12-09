#include <iostream>
#include <limits>
#include <sstream>

#include "BigInt.hpp"

int main() {
  BigInt i("1");
  BigInt j("1");
  BigInt k("2");

  auto l = i + j;
  bool b = k == l;
  std::cout << i << std::endl;
}