#include <iostream>

#include "BigInt.hpp"

int main() {
  BigInt i;
  BigInt j("1231231");
  std::string s = "0001231231";
  BigInt k(s);

  std::cout << i << std::endl;
  std::cout << j << std::endl;
  std::cout << k << std::endl;
}