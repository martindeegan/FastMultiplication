#include <cmath>

#include "BigInt.hpp"

// Need to initialize the state variable
BigInt::MultiplicationMethod BigInt::method =
    BigInt::MultiplicationMethod::Naive;

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

BigInt::BigInt(long i) { *this = BigInt(std::to_string(i)); }

const std::vector<unsigned long> &BigInt::get_coeffs() const { return coeffs; }

BigInt &BigInt::operator=(long i) {
  *this = BigInt(i);
  return *this;
}

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

bool BigInt::operator==(long i) const { return *this == BigInt(i); }

bool BigInt::operator==(std::string &&str) const {
  return *this == BigInt(str);
}

bool BigInt::operator<(const BigInt &other) const {
  if (other.coeffs.size() > coeffs.size()) {
    return false;
  } else if (other.coeffs.size() < coeffs.size()) {
    return true;
  } else {
    for (size_t i = coeffs.size() - 1; i >= 0; i--) {
      if (other.coeffs[i] > coeffs[i]) {
        return true;
      } else if (other.coeffs[i] < coeffs[i]) {
        return false;
      }
    }
  }
  return false;
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
    sum.coeffs.push_back(digit % Base);
    remainder = digit / Base;
  }

  for (; i < coeffs.size(); i++) {
    unsigned long digit = coeffs[i] + remainder;
    sum.coeffs.push_back(digit % Base);
    remainder = digit / Base;
  }

  if (remainder == 0) {
    sum.coeffs.resize(coeffs.size());
  } else {
    sum.coeffs.push_back(remainder);
  }

  return sum;
}

BigInt BigInt::operator-(const BigInt &other) const {
  // TODO: Implement
  BigInt difference;
  return difference;
}

BigInt BigInt::operator-() const {
  // TODO: Implement
  BigInt negation;
  return negation;
}

BigInt &BigInt::operator+=(const BigInt &other) {
  *this = *this + other;
  return *this;
}

BigInt BigInt::operator*(const BigInt &other) const {
  if (other == 0 || *this == 0) {
    return 0;
  }
  switch (method) {
  case MultiplicationMethod::Naive:
    NaiveMultiplier naive_mult;
    return naive_mult(*this, other);
  case MultiplicationMethod::Karatsuba:
    KaratsubaMultiplier kara_mult;
    return kara_mult(*this, other);
  case MultiplicationMethod::FFT:
    FFTMultiplier fft_mult;
    return fft_mult(*this, other);
  }

  return BigInt();
}

BigInt &BigInt::operator*=(const BigInt &other) {
  *this = *this * other;
  return *this;
}

void BigInt::set_zero() {
  coeffs.resize(1);
  coeffs[0] = 0;
}

std::ostream &operator<<(std::ostream &os, const BigInt &i) {
  auto &coeffs = i.get_coeffs();
  for (auto rit = coeffs.rbegin(); rit != coeffs.rend(); rit++) {
    os << *rit;
  }
  return os;
}
