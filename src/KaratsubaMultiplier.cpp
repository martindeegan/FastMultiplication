#include "BigInt.hpp"

BigInt BigInt::KaratsubaMultiplier::operator()(const BigInt &i,
                                               const BigInt &j) {
  BigInt product = i + j;
  return product;
}