#include "BigInt.hpp"

BigInt BigInt::NaiveMultiplier::operator()(const BigInt &i, const BigInt &j) {
  BigInt product;
  product.coeffs.reserve(i.coeffs.size() + j.coeffs.size() + 1);
  BigInt zeros;
  zeros.coeffs.clear();
  for (size_t k = 0; k < i.coeffs.size(); k++) {
    BigInt sub_product = zeros;
    unsigned long remainder = 0;
    for (size_t l = 0; l < j.coeffs.size(); l++) {
      unsigned long digit = i.coeffs[k] * j.coeffs[l] + remainder;
      sub_product.coeffs.push_back(digit % 10);
      remainder = digit / 10;
    }
    product += sub_product;
    zeros.coeffs.push_back(0);
  }
  return product;
}