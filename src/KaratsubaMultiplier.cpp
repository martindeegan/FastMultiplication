#include <algorithm>
#include <cmath>
#include <iostream>

#include "BigInt.hpp"

KaratsubaMultiplier::SplitInt KaratsubaMultiplier::split(const BigInt &i) {
  size_t n = i.coeffs.size();
  size_t mid = n / 2;

  BigInt lo;
  lo.parity = i.parity;
  lo.coeffs = std::vector<Scalar>(i.coeffs.begin(), i.coeffs.begin() + mid);
  BigInt hi;
  hi.parity = i.parity;
  hi.coeffs = std::vector<Scalar>(i.coeffs.begin() + mid, i.coeffs.end());

  // Set parity to zero if necessary
  auto lo_it = std::find_if_not(lo.coeffs.begin(), lo.coeffs.end(),
                                [](Scalar x) { return x == 0; });
  auto hi_it = std::find_if_not(hi.coeffs.begin(), hi.coeffs.end(),
                                [](Scalar x) { return x == 0; });
  if (lo_it == lo.coeffs.end())
    lo.parity = BigInt::Parity::Zero;
  if (hi_it == hi.coeffs.end())
    hi.parity = BigInt::Parity::Zero;

  SplitInt pair;
  pair.hi = hi;
  pair.lo = lo;

  return pair;
}

BigInt KaratsubaMultiplier::multiply(const BigInt &i, const BigInt &j) {
  if (i.zero() || j.zero()) {
    BigInt zero_int;
    return zero_int;
  }

  size_t n = std::max(i.coeffs.size(), j.coeffs.size());

  // If not a power of 2
  if (n & (n - 1) != 0 || i.coeffs.size() != j.coeffs.size()) {
    BigInt i_pow2 = i;
    BigInt j_pow2 = j;

    size_t n_upper = upper_power_of_two(n);
    i_pow2.coeffs.resize(n_upper, 0);
    j_pow2.coeffs.resize(n_upper, 0);

    return this->multiply(i_pow2, j_pow2);
  }

  BigInt product;
  product.coeffs.clear();

  // Recusion base case. If the integers have only base_size digits, use naive
  // multiplication
  size_t base_size = 1;
  if (n == base_size) {
    BigInt i_copy = i;
    BigInt j_copy = j;
    i_copy.method = BigInt::MultiplicationMethod::Naive;
    j_copy.method = BigInt::MultiplicationMethod::Naive;
    BigInt product = i * j;
    product.method = BigInt::MultiplicationMethod::Karatsuba;
    return product;
  }

  SplitInt split_i = split(i);
  SplitInt split_j = split(j);
  BigInt z_0 = this->multiply(split_i.lo, split_j.lo);
  BigInt z_2 = this->multiply(split_i.hi, split_j.hi);
  BigInt z_1 =
      this->multiply((split_i.hi + split_i.lo), (split_j.hi + split_j.lo)) -
      z_2 - z_0;
  // Multiply by powers of 10;
  z_1.coeffs.insert(z_1.coeffs.begin(), n / 2, 0);
  z_2.coeffs.insert(z_2.coeffs.begin(), n, 0);

  product = z_0 + z_1 + z_2;

  return product;
}