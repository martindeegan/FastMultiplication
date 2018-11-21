#include "BigInt.hpp"

BigInt::BigInt() { set_zero(); }

BigInt::BigInt(const std::string &str) {

  if (str.empty()) {
    set_zero();
    return;
  }

  size_t first_digit_location = str.find_first_not_of('0', 0);
  coeffs.reserve(str.size() - first_digit_location);
  for (auto rit = str.rbegin(); rit != str.rend() - first_digit_location;
       rit++) {
    unsigned long digit = (*rit) - '0';
    coeffs.push_back(digit);
  }
}

BigInt::BigInt(std::string &&str) {
  if (str.empty()) {
    set_zero();
    return;
  }

  size_t first_digit_location = str.find_first_not_of('0', 0);
  coeffs.reserve(str.size() - first_digit_location);
  for (auto rit = str.rbegin(); rit != str.rend() - first_digit_location;
       rit++) {
    unsigned long digit = (*rit) - '0';
    coeffs.push_back(digit);
  }
}

const std::vector<unsigned long> &BigInt::get_coeffs() const { return coeffs; }

void BigInt::set_zero() {
  coeffs.resize(1);
  coeffs[0] = 0;
}
