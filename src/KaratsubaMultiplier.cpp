#include "BigInt.hpp"

KaratsubaMultiplier::SplitInt KaratsubaMultiplier::split(const BigInt &i) {
  size_t n = i.coeffs.size();
  size_t mid = n / 2;
  if (n % 2 == 0) {
    mid++;
  }

  SplitInt pair;
  pair.top;
}

BigInt KaratsubaMultiplier::operator()(const BigInt &i, const BigInt &j) {
  BigInt product;

  // SplitInt split_i = split(i);
  // SplitInt split_j = split(j);
  // BigInt z_0 = split_i.bottom * split_j.bottom;
  // BigInt z_2 = split_i.top * split_j.top;
  // BigInt z_1 = (split_i.top + split_i.bottom) * (split_j.top +
  // split_j.bottom) -
  //              z_2 - z_0;

  // product = z_0;
  // z_1.coeffs.insert()

  return product;
}