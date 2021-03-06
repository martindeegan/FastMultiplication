#include <iostream>
#include <limits>
#include <sstream>

#include "BigInt.hpp"

int main() {
  BigInt::set_mult_method(BigInt::MultiplicationMethod::Karatsuba);
  std::string istr;
  std::string jstr;
  std::getline(std::cin, istr);
  std::getline(std::cin, jstr);
  BigInt i(istr);
  BigInt j(jstr);

  std::cout << i * j << std::endl;
}