#define BOOST_TEST_MODULE FFTMultTest

#include <sstream>

#include <boost/test/unit_test.hpp>

#include "BigInt.hpp"

void set_default() { DefaultMultMethod = BigInt::MultiplicationMethod::FFT; }

BOOST_AUTO_TEST_CASE(MultTest1) {
  set_default();
  BigInt i;
  BigInt j;

  BigInt result;

  BOOST_CHECK_EQUAL(i * j, result);
}
