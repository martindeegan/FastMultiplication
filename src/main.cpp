#include <iostream>
#include <limits>
#include <sstream>

#include "BigInt.hpp"

int main() {
  DefaultMultMethod = BigInt::MultiplicationMethod::Naive;

  BigInt i("98");
  BigInt j("1");
  BigInt k = i + j;
  std::cout << k.get_coeffs().size() << std::endl;
  std::cout << k.get_coeffs().capacity() << std::endl;

  k = k + j;
  std::cout << k.get_coeffs().size() << std::endl;
  std::cout << k.get_coeffs().capacity() << std::endl;
}