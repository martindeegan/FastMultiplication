#ifndef BIG_INT_HPP
#define BIG_INT_HPP

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

class NaiveMultiplier;
class KaratsubaMultiplier;
class FFTMultiplier;

using Scalar = int16_t;

class BigInt {

public:
  enum class MultiplicationMethod { Naive, Karatsuba, FFT };
  enum class Parity { Positive, Negative, Zero };

  static constexpr Scalar Base = 10;

public:
  BigInt();
  BigInt(const std::string &str);
  BigInt(std::string &&str);
  // BigInt(const char *str);
  BigInt(long i);

  void initialize_string(std::string trimmed);

  const std::vector<Scalar> &get_coeffs() const;
  bool negative() const;
  bool positive() const;
  bool zero() const;

  BigInt &operator=(long i);
  bool operator<(const BigInt &other) const;
  bool operator<(long i) const;
  bool operator<(std::string &&str) const;

  bool operator<=(const BigInt &other) const;
  bool operator<=(long i) const;
  bool operator<=(std::string &&str) const;

  bool operator>(const BigInt &other) const;
  bool operator>(long i) const;
  bool operator>(std::string &&str) const;

  bool operator>=(const BigInt &other) const;
  bool operator>=(long i) const;
  bool operator>=(std::string &&str) const;

  bool operator==(const BigInt &other) const;
  bool operator==(long i) const;
  bool operator==(std::string &&str) const;

  bool operator!=(const BigInt &other) const;
  bool operator!=(long i) const;
  bool operator!=(std::string &&str) const;

  BigInt operator+(const BigInt &other) const;
  BigInt operator-(const BigInt &other) const;
  BigInt operator-() const;
  BigInt &operator+=(const BigInt &other);
  BigInt operator*(const BigInt &other) const;
  BigInt &operator*=(const BigInt &other);

  static MultiplicationMethod method;

  static void set_mult_method(MultiplicationMethod mm) { BigInt::method = mm; }

private:
  friend NaiveMultiplier;

  friend KaratsubaMultiplier;

  friend FFTMultiplier;

private:
  void trim_coeff();

  void set_zero();

private:
  Parity parity;

  std::vector<Scalar> coeffs;
};

class NaiveMultiplier {
public:
  NaiveMultiplier() = default;

  BigInt operator()(const BigInt &i, const BigInt &j);
};

size_t upper_power_of_two(size_t v);

class KaratsubaMultiplier {
public:
  KaratsubaMultiplier() = default;

  struct SplitInt {
    BigInt hi;
    BigInt lo;
  };
  SplitInt split(const BigInt &i);
  BigInt multiply(const BigInt &i, const BigInt &j);
};

class FFTMultiplier {
public:
  FFTMultiplier() = default;

  struct SplitInt {
    BigInt even;
    BigInt odd;
  };
  SplitInt split(const BigInt &i);

  size_t get_pow_two(const BigInt &i, const BigInt &j);

  std::vector<Scalar> FFT(const BigInt &i, size_t n);

  BigInt invFFT(std::vector<Scalar> &y);

  void FFTUtil(std::vector<Scalar> &v, std::vector<Scalar> &y, Scalar w);

  BigInt operator()(const BigInt &i, const BigInt &j);
};

std::string trim(const std::string &str);
bool my_isdigit(char c);

std::ostream &operator<<(std::ostream &os, const BigInt &i);

#endif
