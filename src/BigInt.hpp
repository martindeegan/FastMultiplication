#pragma once

#include <ostream>
#include <string>
#include <vector>

class BigInt {

  constexpr static unsigned long noint = -1;

public:
  BigInt();

  BigInt(const std::string &str);

  BigInt(std::string &&str);

  const std::vector<unsigned long> &get_coeffs() const;

  bool operator==(const BigInt &other) const;

  BigInt operator+(const BigInt &other) const;

  BigInt &operator+=(const BigInt &other);

private:
  void set_zero();

  std::vector<unsigned long> coeffs;
};

std::ostream &operator<<(std::ostream &os, const BigInt &i) {
  auto &coeffs = i.get_coeffs();
  for (auto rit = coeffs.rbegin(); rit != coeffs.rend(); rit++) {
    os << *rit;
  }
  return os;
}
