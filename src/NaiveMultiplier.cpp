#include "BigInt.hpp"

BigInt BigInt::NaiveMultiplier::operator()(const BigInt &i, const BigInt &j) {
  BigInt product = i + j;
  return product;
}