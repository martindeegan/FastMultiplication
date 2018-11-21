#include "BigInt.hpp"

BigInt::BigInt() : coeffs(1, 0) {}

BigInt::BigInt(const std::string &str) {
  size_t first_digit_location = str.find_first_not_of('0', 0);
  coeffs.reserve(str.size() - first_digit_location);
  for (auto rit = str.rbegin(); rit != str.rend(); rit++) {
    unsigned long digit = (*rit) - '0';
    coeffs.push_back(digit);
  }

  if (coeffs.size() == 0) {
    coeffs.push_back(0);
  }
}

BigInt::BigInt(std::string &&str) {
  size_t first_digit_location = str.find_first_not_of('0', 0);
  coeffs.reserve(str.size() - first_digit_location);
  for (auto rit = str.rbegin(); rit != str.rend(); rit++) {
    unsigned long digit = (*rit) - '0';
    coeffs.push_back(digit);
  }

  if (coeffs.size() == 0) {
    coeffs.push_back(0);
  }
}

const std::vector<unsigned long> &BigInt::get_coeffs() const { return coeffs; }
