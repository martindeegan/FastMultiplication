#include "BigInt.hpp"
#include <iostream>
#include <sstream>

int main() {
  BigInt i("98");
  BigInt j("1");
  BigInt k = i + j;
  std::cout << k.get_coeffs().size() << std::endl;
  std::cout << k.get_coeffs().capacity() << std::endl;

  k = k + j;
  std::cout << k.get_coeffs().size() << std::endl;
  std::cout << k.get_coeffs().capacity() << std::endl;
}