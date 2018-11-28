#ifndef BIG_INT_HPP
#define BIG_INT_HPP

#include <ostream>
#include <string>
#include <vector>

class BigInt {

public:
  enum class MultiplicationMethod { Naive, Karatsuba, FFT };

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
  BigInt &operator+=(const BigInt &other);
  BigInt operator*(const BigInt &other) const;
  BigInt &operator*=(const BigInt &other);

  void set_mult_method(MultiplicationMethod mm);

private:
  class NaiveMultiplier {
  public:
    NaiveMultiplier() = default;

    BigInt operator()(const BigInt &i, const BigInt &j);
  };
  friend NaiveMultiplier;

  class KaratsubaMultiplier {
  public:
    KaratsubaMultiplier() = default;

    BigInt operator()(const BigInt &i, const BigInt &j);
  };
  friend KaratsubaMultiplier;

  class FFTMultiplier {
  public:
    FFTMultiplier() = default;

    BigInt operator()(const BigInt &i, const BigInt &j);
  };
  friend FFTMultiplier;

private:
  void set_zero();

private:
  // bool negative;
  std::vector<unsigned long> coeffs;
  MultiplicationMethod method;
};

std::ostream &operator<<(std::ostream &os, const BigInt &i);

static BigInt::MultiplicationMethod DefaultMultMethod =
    BigInt::MultiplicationMethod::Naive;

#endif
