#include <stdio.h>
#include <iostream>
using namespace std;

#include <FakeSerial.hpp>
#include <SDIBusController.hpp>
#include <SDIRemoteSensor.hpp>

#include <gtest/gtest.h>

class FakeSerialTest: public ::testing::Test {
public:
  FakeSerialTest() {

  }

  virtual void SetUp() {
    Serial1.clearInputBuffer();
    Serial1.clearOutputHistory();
  }

  virtual void TearDown() {
  }
};

TEST_F(FakeSerialTest, beginend) {
  Serial1.begin(9600, SERIAL_7E1);
  ASSERT_EQ(Serial1.getSerialType(), SERIAL_7E1);
  ASSERT_TRUE(Serial1.active());

  Serial1.end();
  ASSERT_FALSE(Serial1.active());
}

TEST_F(FakeSerialTest, write) {
  ASSERT_EQ(Serial1.getOutputHistory().length(), 0);

  Serial1.write('a');
  ASSERT_EQ(Serial1.getOutputHistory().length(), 1);
  ASSERT_EQ(Serial1.getOutputHistory()[0], 'a');

  char message[] = "message";
  Serial1.write(message);
  ASSERT_EQ(Serial1.getOutputHistory().length(), 8);
  ASSERT_STREQ(Serial1.getOutputHistory().c_str(), "amessage");

  Serial1.clearOutputHistory();
  ASSERT_EQ(Serial1.getOutputHistory().length(), 0);
}

TEST_F(FakeSerialTest, read) {
  ASSERT_FALSE(Serial1.available());

  Serial1.addToInputBuffer("abc");
  ASSERT_EQ(Serial1.available(), 3);

  // make sure we are reading out the right value
  ASSERT_EQ(Serial1.read(), 'a');
  ASSERT_EQ(Serial1.available(), 2);
  ASSERT_EQ(Serial1.read(), 'b');
  ASSERT_EQ(Serial1.available(), 1);
  ASSERT_EQ(Serial1.read(), 'c');
  ASSERT_EQ(Serial1.available(), 0);
}
