#include "BigInt.hpp"

#include <cmath>

FFTMultiplier::SplitInt FFTMultiplier::split(const BigInt &i) {
  SplitInt s;
  s.even.coeffs.clear();
  s.odd.coeffs.clear();
  for (size_t j = 0; j < i.coeffs.size(); j++) {
    if (j % 2 == 0) {
      s.even.coeffs.push_back(i.coeffs[j]);
    } else {
      s.odd.coeffs.push_back(i.coeffs[j]);
    }
  }
}

size_t FFTMultiplier::get_pow_two(const BigInt &i, const BigInt &j) {
  double max_log =
      std::max(std::log2(i.coeffs.size()), std::log2(j.coeffs.size()));
  return static_cast<size_t>(std::ceil(max_log));
}

std::vector<Scalar> FFTMultiplier::FFT(const BigInt &i, size_t n) {
  std::vector<Scalar> y(n);
  std::vector<Scalar> v(n, 0);
  std::copy(i.coeffs.begin(), i.coeffs.end(), v.begin());
  FFTUtil(v, y, 1);
  return y;
}

BigInt FFTMultiplier::invFFT(std::vector<Scalar> &y) {
  BigInt result;
  result.coeffs.resize(y.size());
  FFTUtil(y, result.coeffs, -1);

  return result;
}

void FFTMultiplier::FFTUtil(std::vector<Scalar> &v, std::vector<Scalar> &y,
                            Scalar w) {}

BigInt FFTMultiplier::operator()(const BigInt &i, const BigInt &j) {
  size_t n = get_pow_two(i, j);

  auto y1 = FFT(i, n);
  auto y2 = FFT(j, n);

  for (size_t k = 0; k < y1.size(); k++) {
    y1[k] *= y2[k];
  }

  BigInt product = invFFT(y1);

  return product;
}
