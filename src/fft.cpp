#include <cmath>
#include <complex>
#include <iostream>
#include <valarray>
#include <vector>

constexpr int t = 1;
static int m = 0;

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

using Scalar = std::complex<double>;

// void FFTUtil(std::vector<Scalar> &a, std::vector<Scalar> &y, Scalar w_n,
//              size_t l, size_t offset) {
//   size_t n = a.size() / l;
//   if (n == 1) {
//     y[0] = a[offset];
//     return;
//   }

//   std::vector<Scalar> y_0(n / 2);
//   std::vector<Scalar> y_1(n / 2);
//   FFTUtil(a, y_0, w_n, l * 2, offset);
//   FFTUtil(a, y_1, w_n, l * 2, offset + l);

//   Scalar w = 1;
//   for (size_t k = 0; k < n; k++) {
//     y[k] = y_0[k] + w * y_1[k];
//     y[k + n / 2] = y_0[k] - w * y_1[k];
//     w = w * w_n;
//   }
// }

void FFT(CArray &x) {
  const size_t N = x.size();
  if (N <= 1)
    return;

  // divide
  CArray even = x[std::slice(0, N / 2, 2)];
  CArray odd = x[std::slice(1, N / 2, 2)];

  // conquer
  FFT(even);
  FFT(odd);

  // combine
  for (size_t k = 0; k < N / 2; ++k) {
    Complex t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
    x[k] = even[k] + t;
    x[k + N / 2] = even[k] - t;
  }
}

void invFFT(CArray &x) {
  // conjugate the complex numbers
  x = x.apply(std::conj);

  // forward fft
  FFT(x);

  // conjugate the complex numbers again
  x = x.apply(std::conj);

  // scale the numbers
  x /= x.size();
}

void multiply(std::valarray<Scalar> &i, std::valarray<Scalar> &j) {

  FFT(i);
  FFT(j);

  for (size_t k = 0; k < i.size(); k++) {
    i[k] *= j[k];
  }

  invFFT(i);
}

size_t upper_power_of_two(size_t v) {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v |= v >> 32;
  v++;
  return v;
}

int main() {
  std::string istr;
  std::string jstr;
  //   istr = "12";
  //   jstr = "34";
  std::getline(std::cin, istr);
  std::getline(std::cin, jstr);

  size_t n = 2 * upper_power_of_two(std::max(istr.size(), jstr.size()));
  std::vector<Scalar> i_vec;
  std::vector<Scalar> j_vec;
  i_vec.reserve(n);
  j_vec.reserve(n);
  for (auto it = istr.rbegin(); it != istr.rend(); it++) {
    i_vec.push_back(static_cast<double>(*it - '0'));
  }
  for (auto it = jstr.rbegin(); it != jstr.rend(); it++) {
    j_vec.push_back(static_cast<double>(*it - '0'));
  }
  while (i_vec.size() < n) {
    i_vec.push_back(0);
  }
  while (j_vec.size() < n) {
    j_vec.push_back(0);
  }

  for (auto x : i_vec) {
    std::cout << x;
  }
  std::cout << std::endl;

  for (auto x : j_vec) {
    std::cout << x;
  }
  std::cout << std::endl;
  std::valarray<Complex> i(i_vec.data(), i_vec.size());
  std::valarray<Complex> j(j_vec.data(), j_vec.size());
  multiply(i, j);
  i_vec.assign(std::begin(i), std::end(i));
  for (auto it = i_vec.rbegin(); it != i_vec.rend(); it++) {
    std::cout << *it;
  }
  std::cout << std::endl;
}