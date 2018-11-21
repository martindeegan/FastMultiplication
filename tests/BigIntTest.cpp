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
  std::string int_string = "00012300123";
  BigInt i(int_string);
  std::stringstream ss;
  ss << i;
  BOOST_CHECK_EQUAL(ss.str(), "12300123");
}

BOOST_AUTO_TEST_CASE(ConstructorTestCase3) {
  std::string int_string = "";
  BigInt i(int_string);
  std::stringstream ss;
  ss << i;
  BOOST_CHECK_EQUAL(ss.str(), "0");
}

BOOST_AUTO_TEST_CASE(ConstructorTestCase4) {
  BigInt i("00012300123");
  std::stringstream ss;
  ss << i;
  BOOST_CHECK_EQUAL(ss.str(), "12300123");
}

BOOST_AUTO_TEST_CASE(ConstructorTestCase5) {
  BigInt i("");
  std::stringstream ss;
  ss << i;
  BOOST_CHECK_EQUAL(ss.str(), "0");
}

BOOST_AUTO_TEST_CASE(ConstructorTestCase6) {
  std::string int_string = "1234567890";
  BigInt i(int_string);
  std::stringstream ss;
  ss << i;
  BOOST_CHECK_EQUAL(ss.str(), "1234567890");
}

BOOST_AUTO_TEST_CASE(ConstructorTestCase7) {
  BigInt i("1234567890");
  std::stringstream ss;
  ss << i;
  BOOST_CHECK_EQUAL(ss.str(), "1234567890");
}
