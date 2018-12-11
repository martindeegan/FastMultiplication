#include <chrono>
#include <iostream>
#include <random>
#include <string>

#include "BigInt.hpp"

double run_benchmark(const std::string &str1, const std::string &str2,
                     BigInt::MultiplicationMethod method) {
  BigInt::set_mult_method(method);
  BigInt i1(str1);
  BigInt i2(str2);
  constexpr size_t iterations = 5;
  auto start = std::chrono::system_clock::now();
  for (size_t i = 0; i < iterations; i++) {
    BigInt j = i1 * i2;
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
  constexpr size_t max_n = 65536;
  constexpr size_t num_increase = 100;
  std::string s1 = "";
  std::string s2 = "";
  for (size_t i = 1; i < max_n; i += num_increase) {
    std::cerr << i << std::endl;
    add_digit(s1, num_increase);
    add_digit(s2, num_increase);
    double naive = run_benchmark(s1, s2, BigInt::MultiplicationMethod::Naive);
    double karatsubas = 0;
    // run_benchmark(s1, s2, BigInt::MultiplicationMethod::Karatsuba);
    double fft = 0;
    std::cout << i << ", " << naive << ", " << karatsubas << ", " << fft
              << '\n';
  }
}