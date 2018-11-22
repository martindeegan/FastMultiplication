#include <cmath>

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

//==============================================================================
//============================= Operator Overloads =============================
//==============================================================================

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

BigInt BigInt::operator+(const BigInt &other) const {
  // This should always be the largest int
  if (coeffs.size() < other.coeffs.size()) {
    return other + *this;
  }

  BigInt sum;
  sum.coeffs.clear();

  size_t size = coeffs.size() + 1;
  sum.coeffs.reserve(size);

  size_t i = 0;
  unsigned long remainder = 0;

  for (; i < other.coeffs.size(); i++) {
    unsigned long digit = coeffs[i] + other.coeffs[i] + remainder;
    sum.coeffs.push_back(digit % 10);
    remainder = digit / 10;
  }

  for (; i < coeffs.size(); i++) {
    unsigned long digit = coeffs[i] + remainder;
    sum.coeffs.push_back(digit % 10);
    remainder = digit / 10;
  }

  if (remainder == 0) {
    sum.coeffs.resize(coeffs.size());
  } else {
    sum.coeffs.push_back(remainder);
  }

  return sum;
}

BigInt &BigInt::operator+=(const BigInt &other) {
  *this = *this + other;
  return *this;
}

void BigInt::set_zero() {
  coeffs.resize(1);
  coeffs[0] = 0;
}
