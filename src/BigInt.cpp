#include <algorithm>
#include <cmath>
#include <iostream>

#include "BigInt.hpp"

// Need to initialize the state variable
BigInt::MultiplicationMethod BigInt::method =
    BigInt::MultiplicationMethod::Naive;

// =============================================================================
// Utility
// =============================================================================

std::string trim(const std::string &str) {
  std::string trimmed = str;
  trimmed.erase(std::remove_if(trimmed.begin(), trimmed.end(), my_isdigit),
                trimmed.end());
  return trimmed;
}

bool my_isdigit(char c) { return !(std::isdigit(c) || c == '-'); }

size_t upper_power_of_two(size_t v) {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v |= v >> 32;
  v++;
  return v;
}

// =============================================================================
// Constructors
// =============================================================================

void BigInt::initialize_string(std::string trimmed) {
  size_t first_nonzero = trimmed.find_first_not_of('0', 0);

  // Check for negative;
  if (trimmed[first_nonzero] == '-') {
    parity = Parity::Negative;
    first_nonzero = trimmed.find_first_not_of('0', first_nonzero + 1);
  }

  // Check for empty string
  if (first_nonzero == std::string::npos) {
    set_zero();
    return;
  }

  coeffs.reserve(trimmed.size() - first_nonzero);
  for (auto rit = trimmed.rbegin(); rit != trimmed.rend() - first_nonzero;
       rit++) {
    Scalar digit = (*rit) - '0';
    coeffs.push_back(digit);
  }
}

BigInt::BigInt() { set_zero(); }

BigInt::BigInt(const std::string &str) : parity(Parity::Positive) {
  initialize_string(trim(str));
}

BigInt::BigInt(std::string &&str) : parity(Parity::Positive) {
  initialize_string(trim(str));
}

BigInt::BigInt(long i) { *this = BigInt(std::to_string(i)); }

// =============================================================================
// Getters
// =============================================================================

const std::vector<Scalar> &BigInt::get_coeffs() const { return coeffs; }

bool BigInt::negative() const { return parity == Parity::Negative; }
bool BigInt::positive() const { return parity == Parity::Positive; }
bool BigInt::zero() const { return parity == Parity::Zero; }

// =============================================================================
// Comparators
// =============================================================================

BigInt &BigInt::operator=(long i) {
  *this = BigInt(i);
  return *this;
}

bool BigInt::operator==(const BigInt &other) const {
  // Quick parity check
  if (parity != other.parity) {
    return false;
  }

  // If both are zero
  if (zero()) {
    return true;
  }

  // Quick order of magnitude check
  if (other.coeffs.size() != coeffs.size()) {
    return false;
  }

  // Check each coefficient
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
  // Quick pairity check
  if (parity != other.parity) {
    if (negative())
      return true;
    if (positive())
      return false;
    if (zero())
      return other.negative();
  }

  if (zero())
    // If both are zero
    return false;
  else if (positive()) {
    // Positive case
    if (other.coeffs.size() > coeffs.size()) {
      // Other contains more digits, therefore more positive and greater
      return true;
    } else if (other.coeffs.size() < coeffs.size()) {
      // this contains more digits, therefore, more positive and greater
      return false;
    } else {
      for (auto rit_this = this->coeffs.rbegin(),
                rit_other = other.coeffs.rbegin();
           rit_this != this->coeffs.rend() && rit_other != other.coeffs.rend();
           rit_this++, rit_other++) {
        if (*rit_this < *rit_other) {
          // this is less
          return true;
          break;
        } else if (*rit_this > *rit_other) {
          // other is less
          return false;
          break;
        }
      }
    }
  } else if (negative()) {
    // Negative case
    if (other.coeffs.size() > coeffs.size()) {
      // Other contains more digits therefore more negative and lesser
      return false;
    } else if (other.coeffs.size() < coeffs.size()) {
      // this contains more digits, therefore more negative and lesser
      return true;
    } else {
      for (auto rit_this = this->coeffs.rbegin(),
                rit_other = other.coeffs.rbegin();
           rit_this != this->coeffs.rend() && rit_other != other.coeffs.rend();
           rit_this++, rit_other++) {
        if (*rit_this < *rit_other) {
          // other is less
          return false;
          break;
        } else if (*rit_this > *rit_other) {
          // this is less
          return true;
          break;
        }
      }
    }
  }

  return false;
}
bool BigInt::operator<(long i) const { return *this < BigInt(i); }
bool BigInt::operator<(std::string &&str) const { return *this < BigInt(str); }

bool BigInt::operator<=(const BigInt &other) const {
  return *this < other || *this == other;
}
bool BigInt::operator<=(long i) const { return *this <= BigInt(i); }
bool BigInt::operator<=(std::string &&str) const {
  return *this <= BigInt(str);
}

bool BigInt::operator!=(const BigInt &other) const { return !(*this == other); }
bool BigInt::operator!=(long i) const { return *this != BigInt(i); }
bool BigInt::operator!=(std::string &&str) const {
  return *this != BigInt(str);
}

bool BigInt::operator>(const BigInt &other) const {
  return !(*this < other || *this == other);
}
bool BigInt::operator>(long i) const { return *this > BigInt(i); }
bool BigInt::operator>(std::string &&str) const { return *this > BigInt(str); }

bool BigInt::operator>=(const BigInt &other) const { return !(*this < other); }
bool BigInt::operator>=(long i) const { return *this >= BigInt(i); }
bool BigInt::operator>=(std::string &&str) const {
  return *this >= BigInt(str);
}

// =============================================================================
// Aritmetic
// =============================================================================

void BigInt::trim_coeff() {
  auto it = std::find_if(coeffs.rbegin(), coeffs.rend(),
                         [](Scalar i) { return i != 0; });

  if (it == coeffs.rend()) {
    set_zero();
    return;
  }

  size_t diff = coeffs.rend() - it;
  coeffs.resize(diff);
}

BigInt BigInt::operator+(const BigInt &other) const {
  // If either are zero return the other
  if (zero())
    return other;
  if (other.zero())
    return *this;

  // this needs to have the most number of digits
  if (positive()) {
    if (other.positive()) {
      if (other > *this) {
        return other + *this;
      }
    } else if (other.negative()) {
      if (-other > *this) {
        return -((-other) + (-*this));
      }
    }
  } else if (negative()) {
    if (other.positive()) {
      if (other > -*this) {
        return -((-other) + (-*this));
      }
    } else if (other.negative()) {
      if (other < *this) {
        return other + *this;
      }
    }
  }

  Scalar pm = (parity == other.parity) ? 1 : -1;

  BigInt sum;
  sum.parity = this->parity;
  sum.coeffs.clear();

  size_t size = coeffs.size() + 1;
  sum.coeffs.reserve(size);

  size_t i = 0;
  Scalar carry = 0;

  for (; i < other.coeffs.size(); i++) {
    Scalar digit = carry + coeffs[i] + (other.coeffs[i] * pm);
    Scalar remainder = digit % Base;
    carry = digit / Base;
    if (remainder < 0) {
      remainder += Base;
      carry--;
    }
    sum.coeffs.push_back(remainder);
  }

  for (; i < coeffs.size(); i++) {
    Scalar digit = coeffs[i] + carry;
    sum.coeffs.push_back(digit % Base);
    carry = digit / Base;
  }

  if (carry == 0) {
    sum.coeffs.resize(coeffs.size());
  } else {
    sum.coeffs.push_back(carry);
  }

  sum.trim_coeff();

  return sum;
}

BigInt BigInt::operator-(const BigInt &other) const { return *this + (-other); }

BigInt BigInt::operator-() const {
  // TODO: Implement
  BigInt negation = *this;
  switch (parity) {
  case Parity::Positive:
    negation.parity = Parity::Negative;
    break;
  case Parity::Negative:
    negation.parity = Parity::Positive;
    break;
  case Parity::Zero:
    negation.parity = Parity::Zero;
    break;
  }
  return negation;
}

BigInt &BigInt::operator+=(const BigInt &other) {
  *this = *this + other;
  return *this;
}

BigInt BigInt::operator*(const BigInt &other) const {
  if (zero() || other.zero()) {
    BigInt zero_int;
    return zero_int;
  }

  BigInt product;
  product.parity = Parity::Positive;

  switch (method) {
  case MultiplicationMethod::Naive:
    NaiveMultiplier naive_mult;
    product = naive_mult(*this, other);
    break;
  case MultiplicationMethod::Karatsuba:
    KaratsubaMultiplier kara_mult;
    product = kara_mult.multiply(*this, other);
    break;
  case MultiplicationMethod::FFT:
    FFTMultiplier fft_mult;
    product = fft_mult(*this, other);
    break;
  }

  if (parity != other.parity) {
    product = -product;
  }

  product.trim_coeff();
  return product;
}

BigInt &BigInt::operator*=(const BigInt &other) {
  *this = *this * other;
  return *this;
}

// =============================================================================
// Setters
// =============================================================================

void BigInt::set_zero() {
  parity = Parity::Zero;
  coeffs.resize(1);
  coeffs[0] = 0;
}

// =============================================================================
// Printer
// =============================================================================

std::ostream &operator<<(std::ostream &os, const BigInt &i) {
  if (i.zero()) {
    os << 0;
    return os;
  }

  if (i.negative()) {
    os << '-';
  }

  auto &coeffs = i.get_coeffs();
  for (auto rit = coeffs.rbegin(); rit != coeffs.rend(); rit++) {
    os << *rit;
  }
  return os;
}
