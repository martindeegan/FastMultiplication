#include <iostream>
#include <limits>
#include <sstream>

#include "BigInt.hpp"

int main() {
  BigInt::set_mult_method(BigInt::MultiplicationMethod::Naive);
  // BigInt i = 123456;
  // Scalar x = 0;
  // BigInt::Iterator it = i.begin();
  // std::vector<Scalar> v = i.get_coeffs();
  // x = *it; // 0
  // ++it;
  // x = *it; // 1
  // ++it;
  // x = *it; // 2
  // it = it + 3;
  // x = *it; // 5
  // ++it;
  // bool b = it == i.end();

  BigInt i = 1;
  BigInt j = 1;

  i + j;
  // BigInt::set_mult_method(BigInt::MultiplicationMethod::Karatsuba);
  // std::string istr;
  // std::string jstr;
  // std::getline(std::cin, istr);
  // std::getline(std::cin, jstr);
  // BigInt i(istr);
  // BigInt j(jstr);

  // std::cout << i * j << std::endl;
}