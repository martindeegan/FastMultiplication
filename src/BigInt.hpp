#ifndef BIG_INT_HPP
#define BIG_INT_HPP

#include <cstdint>
#include <memory>
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
  BigInt(const BigInt &other);
  BigInt(const BigInt &other, size_t start_i, size_t end_i, size_t i_step);

  void initialize_vector();
  void initialize_string(std::string trimmed);

  const std::vector<Scalar> &get_coeffs() const;
  bool negative() const;
  bool positive() const;
  bool zero() const;

  BigInt &operator=(const BigInt &i);
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

  Scalar operator[](size_t index) const;
  size_t size() const;

  class Iterator {
  public:
    Iterator(const BigInt *p, size_t i);
    void operator++();
    Iterator operator+(size_t n);
    size_t operator-(const Iterator &other);
    bool operator==(const Iterator &other);
    bool operator!=(const Iterator &other);
    Scalar operator*();

  private:
    const BigInt *parent;
    size_t index;
  };

  class ReverseIterator {
  public:
    ReverseIterator(const BigInt *p, size_t i);
    void operator++();
    ReverseIterator operator+(size_t n);
    size_t operator-(const ReverseIterator &other);
    bool operator==(const ReverseIterator &other);
    bool operator!=(const ReverseIterator &other);
    Scalar operator*();

  private:
    const BigInt *parent;
    size_t index;
  };

  Iterator begin() const;
  Iterator end() const;
  ReverseIterator rbegin() const;
  ReverseIterator rend() const;

  friend BigInt::Iterator;

  void set_index_bounds(size_t start_i, size_t end_i, size_t i_step);

private:
  friend NaiveMultiplier;

  friend KaratsubaMultiplier;

  friend FFTMultiplier;

private:
  void trim_coeff();

  void set_zero();

private:
  Parity parity;

  std::shared_ptr<std::vector<Scalar>> coeffs;

  bool wrapper;
  size_t start_index;
  size_t end_index;
  size_t index_step;

  size_t power = 0;
};

class NaiveMultiplier {
public:
  static size_t num_multiplications;

  NaiveMultiplier() = default;

  BigInt operator()(const BigInt &i, const BigInt &j);
};

size_t upper_power_of_two(size_t v);

class KaratsubaMultiplier {
public:
  static size_t num_multiplications;

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
  static size_t num_multiplications;

  FFTMultiplier() = default;

  struct SplitInt {
    BigInt even;
    BigInt odd;
  };
  SplitInt split(const BigInt &i);

  BigInt FFT(BigInt &i);
  BigInt invFFT(BigInt &y);

  void FFTUtil(BigInt &v, BigInt &y, size_t n, Scalar w);

  BigInt multiply(const BigInt &i, const BigInt &j);

private:
  size_t m;
};

std::string trim(const std::string &str);
bool my_isdigit(char c);

std::ostream &operator<<(std::ostream &os, const BigInt &i);

#endif
