#include <chrono>
#include <cmath>
#include <complex>
#include <iostream>
#include <random>
#include <string>
#include <valarray>
#include <vector>

constexpr int t = 1;
static int m = 0;

using Scalar = std::complex<double>;

void FFT(std::valarray<Scalar> &x) {
  const size_t N = x.size();
  if (N <= 1)
    return;

  // divide
  std::valarray<Scalar> even = x[std::slice(0, N / 2, 2)];
  std::valarray<Scalar> odd = x[std::slice(1, N / 2, 2)];

  // conquer
  FFT(even);
  FFT(odd);

  // combine
  for (size_t k = 0; k < N / 2; ++k) {
    Scalar t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
    x[k] = even[k] + t;
    x[k + N / 2] = even[k] - t;
  }
}

void invFFT(std::valarray<Scalar> &x) {
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
  std::cout << "Multiplying a " << i.size() << " integer with a " << j.size()
            << " integer." << std::endl;

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

double run_benchmark(const std::string &str1, const std::string &str2) {
  size_t n = 2 * upper_power_of_two(std::max(str1.size(), str2.size()));
  std::vector<Scalar> i_vec;
  std::vector<Scalar> j_vec;
  i_vec.reserve(n);
  j_vec.reserve(n);
  for (auto it = str1.rbegin(); it != str1.rend(); it++) {
    i_vec.push_back(static_cast<double>(*it - '0'));
  }
  for (auto it = str2.rbegin(); it != str2.rend(); it++) {
    j_vec.push_back(static_cast<double>(*it - '0'));
  }
  while (i_vec.size() < n) {
    i_vec.push_back(0);
  }
  while (j_vec.size() < n) {
    j_vec.push_back(0);
  }

  std::valarray<Scalar> i(i_vec.data(), i_vec.size());
  std::valarray<Scalar> j(j_vec.data(), j_vec.size());

  constexpr size_t iterations = 5;
  auto start = std::chrono::system_clock::now();
  for (size_t l = 0; l < iterations; l++) {
    multiply(i, j);
  }
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  return elapsed_seconds.count() / static_cast<double>(iterations);
}

void add_digit(std::string &str, size_t num_increase) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 9);

  for (size_t i = 0; i < num_increase; i++) {
    int next_digit = dis(gen);
    str += std::to_string(next_digit);
  }
}

int main() {
  std::string istr;
  std::string jstr;
  //   istr = "12";
  //   jstr = "34";
  //   constexpr size_t max_n = 65536;
  //   constexpr size_t num_increase = 100;
  //   std::string s1 = "";
  //   std::string s2 = "";
  //   for (size_t i = 1; i < max_n; i += num_increase) {
  //     std::cerr << i << std::endl;
  //     add_digit(s1, num_increase);
  //     add_digit(s2, num_increase);
  //     double fft = run_benchmark(s1, s2);
  //     std::cout << i << ", " << fft << '\n';
  //   }
  std::cout << "Reading file..." << std::endl;
  int lines = 0;
  while (std::getline(std::cin, istr)) {
    if (lines % 10000 == 0) {
      std::cout << "Read line " << lines << '\n';
    }
    jstr = jstr + istr;
    lines++;
  }
  //   std::getline(std::cin, jstr);
  std::cout << "Finished reading file..." << std::endl;

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

  //   for (auto x : i_vec) {
  //     std::cout << x;
  //   }
  //   std::cout << std::endl;

  //   for (auto x : j_vec) {
  //     std::cout << x;
  //   }
  //   std::cout << std::endl;
  std::valarray<Scalar> j(j_vec.data(), j_vec.size());
  std::valarray<Scalar> i(j_vec.data(), j_vec.size());
  auto start = std::chrono::system_clock::now();
  multiply(i, j);
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> seconds_elapsed = end - start;
  i_vec.assign(std::begin(i), std::end(i));
  //   for (auto it = i_vec.rbegin(); it != i_vec.rend(); it++) {
  //     std::cout << *it;
  //   }
  std::cout << "Done in " << seconds_elapsed.count() << " seconds :)"
            << std::endl;
  std::cout << std::endl;
}