#include "BigInt.hpp"
#include <iostream>

BigInt NaiveMultiplier::operator()(const BigInt &i, const BigInt &j) {
  BigInt product;
  product.coeffs.reserve(i.coeffs.size() + j.coeffs.size() + 1);
  BigInt zeros;
  zeros.coeffs.clear();
  for (size_t k = 0; k < i.coeffs.size(); k++) {
    BigInt sub_product = zeros;
    sub_product.parity = BigInt::Parity::Positive;
    Scalar remainder = 0;
    if (i.coeffs[k] == 0)
      sub_product = 0;
    else {
      for (size_t l = 0; l < j.coeffs.size(); l++) {
        Scalar digit = i.coeffs[k] * j.coeffs[l] + remainder;
        sub_product.coeffs.push_back(digit % BigInt::Base);
        remainder = digit / BigInt::Base;
      }
      if (remainder != 0) {
        sub_product.coeffs.push_back(remainder);
      }
    }
    product += sub_product;
    zeros.coeffs.push_back(0);
  }
  return product;
}