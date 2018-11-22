#define BOOST_TEST_MODULE NaiveMultTest

#include <sstream>

#include <boost/test/unit_test.hpp>

#include "BigInt.hpp"

void set_default() { DefaultMultMethod = BigInt::MultiplicationMethod::Naive; }

BOOST_AUTO_TEST_CASE(MultTest1) {
  set_default();
  BigInt i;
  BigInt j;

  BigInt result;

  BOOST_CHECK_EQUAL(i * j, result);
  BOOST_CHECK_EQUAL(j * i, result);
}

BOOST_AUTO_TEST_CASE(MultTest2) {
  set_default();
  BigInt i("1");
  BigInt j("100");

  BigInt result("100");

  BOOST_CHECK_EQUAL(i * j, result);
  BOOST_CHECK_EQUAL(j * i, result);
}

BOOST_AUTO_TEST_CASE(MultTest3) {
  set_default();
  BigInt i("1234");
  BigInt j("123");

  BigInt result("151782");

  BOOST_CHECK_EQUAL(i * j, result);
  BOOST_CHECK_EQUAL(j * i, result);
}

BOOST_AUTO_TEST_CASE(MultTest4) {
  set_default();
  BigInt i("1234");
  BigInt j("0");

  BigInt result("0");

  BOOST_CHECK_EQUAL(i * j, result);
  BOOST_CHECK_EQUAL(j * i, result);
}

BOOST_AUTO_TEST_CASE(MultTest5) {
  set_default();
  BigInt i("1180591620717411303424");
  BigInt j("2361183241434822606848");

  BigInt result("2787593149816327892691964784081045188247552");

  BOOST_CHECK_EQUAL(i * j, result);
  BOOST_CHECK_EQUAL(j * i, result);
}

BOOST_AUTO_TEST_CASE(MultTest6) {
  set_default();
  BigInt two("2");
  BigInt i("1");
  BigInt j("1");
  for (size_t k = 0; k < 200; k++) {
    i = i * two;
  }

  for (size_t k = 0; k < 400; k++) {
    j = j * two;
  }

  BigInt result("1");

  for (size_t k = 0; k < 200 + 400; k++) {
    result = result * two;
  }

  BOOST_CHECK_EQUAL(i * j, result);
  BOOST_CHECK_EQUAL(j * i, result);
}
