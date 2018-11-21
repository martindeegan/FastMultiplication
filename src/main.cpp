#include "BigInt.hpp"
#include <iostream>
#include <sstream>

int main() {
  std::string int_string = "0";
  BigInt i(int_string);
  std::stringstream ss;
  ss << i;
  std::cout << ss.str() << std::endl;
}