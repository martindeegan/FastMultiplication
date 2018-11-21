#include "BigInt.hpp"

BigInt::BigInt() { set_zero(); }

BigInt::BigInt(const std::string &str) {
  size_t first_digit_location = str.find_first_not_of('0', 0);
  if (first_digit_location == std::string::npos) {
    set_zero();
    return;
  }
  coeffs.reserve(str.size() - first_digit_location);
  for (auto rit = str.rbegin(); rit != str.rend() - first_digit_location;
       rit++) {
    unsigned long digit = (*rit) - '0';
    coeffs.push_back(digit);
  }
}

BigInt::BigInt(std::string &&str) {
  size_t first_digit_location = str.find_first_not_of('0', 0);
  if (first_digit_location == std::string::npos) {
    set_zero();
    return;
  }
  coeffs.reserve(str.size() - first_digit_location);
  for (auto rit = str.rbegin(); rit != str.rend() - first_digit_location;
       rit++) {
    unsigned long digit = (*rit) - '0';
    coeffs.push_back(digit);
  }
}

const std::vector<unsigned long> &BigInt::get_coeffs() const { return coeffs; }

bool BigInt::operator==(const BigInt &other) const {
  if (other.coeffs.size() != coeffs.size()) {
    return false;
  }

  for (size_t i = 0; i < coeffs.size(); i++) {
    if (other.coeffs[i] != coeffs[i])
      return false;
  }

  return true;
}

void BigInt::set_zero() {
  coeffs.resize(1);
  coeffs[0] = 0;
}
