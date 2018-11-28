#include <iostream>
#include <limits>
#include <sstream>

#include "BigInt.hpp"

int main() {
  DefaultMultMethod = BigInt::MultiplicationMethod::Naive;
  std::string istr;
  std::string jstr;
  std::getline(std::cin, istr);
  std::getline(std::cin, jstr);
  BigInt i(istr);
  BigInt j(jstr);

  std::cout << i * j << std::endl;
}