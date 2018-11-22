#include "BigInt.hpp"

BigInt BigInt::FFTMultiplier::operator()(const BigInt &i, const BigInt &j) {
  BigInt product = i + j;
  return product;
}
