#ifndef BIG_INT_HPP
#define BIG_INT_HPP

#include <ostream>
#include <string>
#include <vector>

class NaiveMultiplier;
class KaratsubaMultiplier;
class FFTMultiplier;

class BigInt {

public:
  enum class MultiplicationMethod { Naive, Karatsuba, FFT };

  static constexpr unsigned long Base = 10;

public:
  BigInt();
  BigInt(const std::string &str);
  BigInt(std::string &&str);
  // BigInt(const char *str);
  BigInt(long i);

  const std::vector<unsigned long> &get_coeffs() const;

  BigInt &operator=(long i);
  bool operator<(const BigInt &other) const;
  bool operator==(const BigInt &other) const;
  bool operator==(long i) const;
  bool operator==(std::string &&str) const;
  BigInt operator+(const BigInt &other) const;
  BigInt operator-(const BigInt &other) const;
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
  void set_zero();

private:
  // bool negative;
  std::vector<unsigned long> coeffs;
};

class NaiveMultiplier {
public:
  NaiveMultiplier() = default;

  BigInt operator()(const BigInt &i, const BigInt &j);
};

class KaratsubaMultiplier {
public:
  KaratsubaMultiplier() = default;

  struct SplitInt {
    BigInt top;
    BigInt bottom;
  };
  SplitInt split(const BigInt &i);
  BigInt operator()(const BigInt &i, const BigInt &j);
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

  std::vector<unsigned long> FFT(const BigInt &i, size_t n);

  BigInt invFFT(std::vector<unsigned long> &y);

  void FFTUtil(std::vector<unsigned long> &v, std::vector<unsigned long> &y,
               unsigned long w);

  BigInt operator()(const BigInt &i, const BigInt &j);
};

std::ostream &operator<<(std::ostream &os, const BigInt &i);

#endif
