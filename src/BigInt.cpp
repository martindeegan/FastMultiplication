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
void BigInt::initialize_vector() {
  std::vector<Scalar> emtpy_vec;
  coeffs = std::make_shared<std::vector<Scalar>>(emtpy_vec);
}

void BigInt::initialize_string(std::string trimmed) {
  initialize_vector();
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

  coeffs->reserve(trimmed.size() - first_nonzero);
  for (auto rit = trimmed.rbegin(); rit != trimmed.rend() - first_nonzero;
       rit++) {
    Scalar digit = (*rit) - '0';
    coeffs->push_back(digit);
  }
  end_index = coeffs->size();
}

BigInt::BigInt() : wrapper(false), start_index(0), end_index(0), index_step(1) {
  initialize_vector();
  set_zero();
}

BigInt::BigInt(const std::string &str)
    : parity(Parity::Positive), wrapper(false), start_index(0), end_index(0),
      index_step(1) {
  initialize_string(trim(str));
}

BigInt::BigInt(std::string &&str)
    : parity(Parity::Positive), wrapper(false), start_index(0), end_index(0),
      index_step(1) {
  initialize_string(trim(str));
}

BigInt::BigInt(long i) : BigInt(std::to_string(i)) {}

BigInt::BigInt(const BigInt &other)
    : wrapper(other.wrapper), start_index(other.start_index),
      end_index(other.end_index), index_step(other.index_step) {
  *coeffs = *(other.coeffs);
}

BigInt::BigInt(const BigInt &other, size_t start_i, size_t end_i, size_t i_step)
    : coeffs(other.coeffs), wrapper(true), start_index(start_i),
      end_index(end_i), index_step(i_step) {}

// =============================================================================
// Getters
// =============================================================================

const std::vector<Scalar> &BigInt::get_coeffs() const { return *coeffs; }

bool BigInt::negative() const { return parity == Parity::Negative; }
bool BigInt::positive() const { return parity == Parity::Positive; }
bool BigInt::zero() const { return parity == Parity::Zero; }

// =============================================================================
// Comparators
// =============================================================================

BigInt &BigInt::operator=(const BigInt &i) {

  *this = i;
  std::vector<Scalar> *v =
      new std::vector<Scalar>(i.coeffs->begin(), i.coeffs->end());
  coeffs = std::shared_ptr<std::vector<Scalar>>(v);
  return *this;
}

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
  if (other.size() != size()) {
    return false;
  }

  // Check each coefficient
  for (size_t i = 0; i < size(); i++) {
    if (other[i] != (*this)[i])
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
    if (other.size() > size()) {
      // Other contains more digits, therefore more positive and greater
      return true;
    } else if (other.size() < size()) {
      // this contains more digits, therefore, more positive and greater
      return false;
    } else {
      for (auto rit_this = this->coeffs->rbegin(),
                rit_other = other.coeffs->rbegin();
           rit_this != this->coeffs->rend() &&
           rit_other != other.coeffs->rend();
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
    if (other.size() > size()) {
      // Other contains more digits therefore more negative and lesser
      return false;
    } else if (other.size() < size()) {
      // this contains more digits, therefore more negative and lesser
      return true;
    } else {
      for (auto rit_this = this->coeffs->rbegin(),
                rit_other = other.coeffs->rbegin();
           rit_this != this->coeffs->rend() &&
           rit_other != other.coeffs->rend();
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
  for (auto it = rbegin(); it != rend(); ++it) {
    if (*it != 0) {
      size_t diff = rend() - it;
      coeffs->resize(diff);

      return;
    }
  }

  set_zero();
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
  sum.coeffs->clear();

  size_t n = size() + 1;
  sum.coeffs->reserve(n);

  Scalar carry = 0;
  Iterator it1 = begin(), it2 = other.begin();
  for (; it2 != other.end(); ++it1, ++it2) {
    Scalar digit = carry + *it1 + (*it2 * pm);
    Scalar remainder = digit % Base;
    carry = digit / Base;
    if (remainder < 0) {
      remainder += Base;
      carry--;
    }
    sum.coeffs->push_back(remainder);
  }

  for (; it1 != end(); ++it1) {
    Scalar digit = *it1 + carry;
    sum.coeffs->push_back(digit % Base);
    carry = digit / Base;
  }

  if (carry != 0) {
    sum.coeffs->push_back(carry);
  }

  sum.end_index = n;

  auto v = sum.get_coeffs();

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
    NaiveMultiplier::num_multiplications = 0;
    product = naive_mult(*this, other);
    break;
  case MultiplicationMethod::Karatsuba:
    KaratsubaMultiplier kara_mult;
    KaratsubaMultiplier::num_multiplications = 0;
    product = kara_mult.multiply(*this, other);
    break;
  case MultiplicationMethod::FFT:
    FFTMultiplier fft_mult;
    FFTMultiplier::num_multiplications = 0;
    product = fft_mult.multiply(*this, other);
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
// Indexing Operators
// =============================================================================

BigInt::Iterator::Iterator(const BigInt *p, size_t i) : parent(p), index(i) {}

void BigInt::Iterator::operator++() { (*this) = operator+(1); }

BigInt::Iterator BigInt::Iterator::operator+(size_t n) {
  Iterator new_it = *this;
  if (index + n < parent->end_index)
    new_it.index += n;
  else
    new_it.index = -1;
  return new_it;
}

size_t BigInt::Iterator::operator-(const Iterator &other) {
  return index - other.index;
}

bool BigInt::Iterator::operator==(const Iterator &other) {
  return index == other.index;
}

bool BigInt::Iterator::operator!=(const Iterator &other) {
  return !(index == other.index);
}

Scalar BigInt::Iterator::operator*() { return (*parent)[index]; }

BigInt::ReverseIterator::ReverseIterator(const BigInt *p, size_t i)
    : parent(p), index(i) {}

void BigInt::ReverseIterator::operator++() { (*this) = operator+(1); }

BigInt::ReverseIterator BigInt::ReverseIterator::operator+(size_t n) {
  ReverseIterator new_it = *this;
  if (index - n >= parent->start_index)
    new_it.index -= n;
  else
    new_it.index = -1;
  return new_it;
}

size_t BigInt::ReverseIterator::operator-(const ReverseIterator &other) {
  return index - other.index;
}

bool BigInt::ReverseIterator::operator==(const ReverseIterator &other) {
  return index == other.index;
}
bool BigInt::ReverseIterator::operator!=(const ReverseIterator &other) {
  return !(index == other.index);
}

Scalar BigInt::ReverseIterator::operator*() { return (*parent)[index]; }

Scalar BigInt::operator[](size_t index) const {
  return (*coeffs)[index * index_step];
}
size_t BigInt::size() const { return end_index; }

BigInt::Iterator BigInt::begin() const {
  if (end_index == start_index)
    return Iterator(this, -1);
  else
    return Iterator(this, start_index);
}

BigInt::Iterator BigInt::end() const {
  Iterator it(this, -1);
  return it;
}

BigInt::ReverseIterator BigInt::rbegin() const {
  if (end_index == start_index)
    return ReverseIterator(this, -1);
  else
    return ReverseIterator(this, end_index - 1);
}

BigInt::ReverseIterator BigInt::rend() const {
  ReverseIterator it(this, -1);
  return it;
}

// =============================================================================
// Setters
// =============================================================================

void BigInt::set_zero() { parity = Parity::Zero; }

void BigInt::set_index_bounds(size_t start_i, size_t end_i, size_t i_step) {
  start_index = start_i;
  end_index = end_i;
  index_step = i_step;
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
