#define BOOST_TEST_MODULE BigIntTest

#include <sstream>

#include <boost/test/unit_test.hpp>

#include "BigInt.hpp"

BOOST_AUTO_TEST_CASE(ConstructorTestCase1) {
  BigInt i;
  std::stringstream ss;
  ss << i;
  BOOST_CHECK_EQUAL(ss.str(), "0");
}

BOOST_AUTO_TEST_CASE(ConstructorTestCase2) {
  std::string int_string = "000123123";
  BigInt i(int_string);
  std::stringstream ss;
  ss << i;
  BOOST_CHECK_EQUAL(ss.str(), "123123");
}
