#include "BigInt.hpp"
#include <iostream>

size_t NaiveMultiplier::num_multiplications = 0;

BigInt NaiveMultiplier::operator()(const BigInt &i, const BigInt &j) {
  BigInt product;
  product.coeffs->reserve(i.size() + j.size() + 1);
  BigInt zeros;
  zeros.coeffs->clear();
  for (size_t k = 0; k < i.size(); k++) {
    BigInt sub_product = zeros;
    sub_product.parity = BigInt::Parity::Positive;
    Scalar remainder = 0;
    if (i[k] == 0)
      sub_product = 0;
    else {
      for (size_t l = 0; l < j.size(); l++) {
        NaiveMultiplier::num_multiplications++;
        Scalar digit = i[k] * j[l] + remainder;
        sub_product.coeffs->push_back(digit % BigInt::Base);
        remainder = digit / BigInt::Base;
      }
      if (remainder != 0) {
        sub_product.coeffs->push_back(remainder);
      }
    }
    product += sub_product;
    zeros.coeffs->push_back(0);
  }
  return product;
}