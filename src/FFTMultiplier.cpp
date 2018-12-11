#include "BigInt.hpp"

#include <cmath>

size_t FFTMultiplier::num_multiplications = 0;

FFTMultiplier::SplitInt FFTMultiplier::split(const BigInt &i) {
  BigInt even;
  BigInt odd;
  for (size_t j = 0; j < i.size(); j++) {
    if (j % 2 == 0) {
      even.coeffs->push_back(i[j]);
    } else {
      odd.coeffs->push_back(i[j]);
    }
  }
  SplitInt s;
  s.even = even;
  s.odd = odd;
  return s;
}

BigInt FFTMultiplier::FFT(BigInt &i) {
  BigInt y;
  y.coeffs->resize(i.size());
  Scalar w = 2;
  BigInt i_copy = i;
  FFTUtil(i, y, i.size(), w);
  return y;
}

BigInt FFTMultiplier::invFFT(BigInt &y) {
  BigInt i;
  return i;
}

void FFTMultiplier::FFTUtil(BigInt &v, BigInt &y, size_t n, Scalar w) {
  SplitInt v_split = split(v);
  if (n == 1) {
    return;
  }
}

BigInt FFTMultiplier::multiply(const BigInt &i, const BigInt &j) {
  if (i.zero() || j.zero()) {
    BigInt zero_int;
    return zero_int;
  }

  size_t n = std::max(i.size(), j.size());

  // If not a power of 2
  if (n & (n - 1) != 0 || i.size() != j.size()) {
    BigInt i_pow2 = i;
    BigInt j_pow2 = j;

    size_t n_upper = upper_power_of_two(n);
    m = std::pow(2, n_upper / 2) + 1;
    i_pow2.coeffs->resize(n_upper, 0);
    j_pow2.coeffs->resize(n_upper, 0);

    return this->multiply(i_pow2, j_pow2);
  }

  // auto y1 = FFT(i);
  // auto y2 = FFT(j);

  // for (size_t k = 0; k < y1.size(); k++) {
  //   y1[k] *= y2[k];
  // }

  BigInt product;
  //  = invFFT(y1);

  return product;
}
