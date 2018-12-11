#include <chrono>
#include <iostream>
#include <random>
#include <string>

#include "BigInt.hpp"

size_t run_benchmark(const std::string &str1, const std::string &str2,
                     BigInt::MultiplicationMethod method) {
  BigInt::set_mult_method(method);
  BigInt i1(str1);
  BigInt i2(str2);
  constexpr size_t iterations = 5;
  BigInt j = i1 * i2;
  switch (method) {
  case BigInt::MultiplicationMethod::Naive:
    return NaiveMultiplier::num_multiplications;
  case BigInt::MultiplicationMethod::Karatsuba:
    return KaratsubaMultiplier::num_multiplications;
  case BigInt::MultiplicationMethod::FFT:
    return FFTMultiplier::num_multiplications;
  }
  return 0;
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
  constexpr size_t max_n = 65536;
  constexpr size_t num_increase = 100;
  std::string s1 = "";
  std::string s2 = "";
  for (size_t i = 1; i < max_n; i += num_increase) {
    std::cerr << i << std::endl;
    add_digit(s1, num_increase);
    add_digit(s2, num_increase);
    size_t naive = run_benchmark(s1, s2, BigInt::MultiplicationMethod::Naive);
    size_t karatsubas =
        run_benchmark(s1, s2, BigInt::MultiplicationMethod::Karatsuba);
    size_t fft = 0;
    std::cout << i << ", " << naive << ", " << karatsubas << ", " << fft
              << '\n';
  }
}