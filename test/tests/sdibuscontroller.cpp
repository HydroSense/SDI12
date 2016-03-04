#include <Arduino.h>
#include <SDIBusController.hpp>

#include <gtest/gtest.h>

#define FLOW_CONTROL_PIN 13
SDIBusController* sdiBusPtr;

class SDIBusControllerTest: public ::testing::Test {
public:
  SDIBusControllerTest() {
    sdiBusPtr = new SDIBusController(FLOW_CONTROL_PIN, 1);
  }

  ~SDIBusControllerTest() {
    delete sdiBusPtr;
  }

  virtual void SetUp() {
    Serial1.clearInputBuffer();
    Serial1.clearOutputHistory();
  }

  virtual void TearDown() {
  }
};

TEST_F(SDIBusControllerTest, begin) {
  sdiBusPtr->begin();

  // assure serial is configured properly
  ASSERT_TRUE(Serial1.active());
  ASSERT_EQ(Serial1.getSerialType(), SERIAL_7E1);
  ASSERT_EQ(Serial1.getBaudRate(), 1200);
}

TEST_F(SDIBusControllerTest, addressQuery) {
  Serial1.addToInputBuffer("1\r\n");

  char addr;
  int res = sdiBusPtr->addressQuery(&addr);
  ASSERT_EQ(res, 0);
  ASSERT_EQ(addr, '1');

  Serial1.addToInputBuffer("x\r\n");

  res = sdiBusPtr->addressQuery(&addr);
  ASSERT_EQ(res, 0);
  ASSERT_EQ(addr, 'x');

  ASSERT_TRUE(Serial1.active());
}

TEST_F(SDIBusControllerTest, acknowledgeActivePresent) {
  Serial1.addToInputBuffer("0\r\n");

  int res = sdiBusPtr->acknowledgeActive('0');
  ASSERT_EQ(res, 0);

  ASSERT_TRUE(Serial1.active());
  ASSERT_STREQ(Serial1.getOutputHistory().c_str(), "0!");
}

TEST_F(SDIBusControllerTest, acknowledgeActiveAbsent) {
  int res = sdiBusPtr->acknowledgeActive('0');
  ASSERT_NE(res, 0);
  ASSERT_EQ(SDIBusErrno, TIMEOUT);

  ASSERT_TRUE(Serial1.active());
}

TEST_F(SDIBusControllerTest, acknowledgeActiveBadAddress) {
  int res = sdiBusPtr->acknowledgeActive('\254');
  ASSERT_NE(res, 0);                                  // ensure non-ok return value
  ASSERT_EQ(SDIBusErrno, BAD_ADDRESS);                // ensure proper error number
  ASSERT_EQ(Serial1.getOutputHistory().length(), 0);  // ensure nothing written on serial
}

// TODO(colin): add identify commands

// typical start measurement command
TEST_F(SDIBusControllerTest, refresh) {
  Serial1.addToInputBuffer("a00101\r\n");

  int waitTime; int numExpected;
  int res = sdiBusPtr->refresh('a', 0, &waitTime, &numExpected);
  ASSERT_STREQ(Serial1.getOutputHistory().c_str(), "aC!");
  ASSERT_EQ(waitTime, 1);
  ASSERT_EQ(numExpected, 1);
  ASSERT_EQ(res, 0);
}

// alternative command
TEST_F(SDIBusControllerTest, altRefresh) {
  Serial1.addToInputBuffer("a01002\r\n");

  int waitTime; int numExpected;
  int res = sdiBusPtr->refresh('a', 2, &waitTime, &numExpected);
  ASSERT_STREQ(Serial1.getOutputHistory().c_str(), "aC2!");
  ASSERT_EQ(waitTime, 10);
  ASSERT_EQ(numExpected, 2);
  ASSERT_EQ(res, 0);
}

// instant refresh
TEST_F(SDIBusControllerTest, instRefresh) {
  Serial1.addToInputBuffer("300001\r\n");

  int waitTime; int numExpected;
  int res = sdiBusPtr->refresh('3', 0, &waitTime, &numExpected);
  ASSERT_STREQ(Serial1.getOutputHistory().c_str(), "3C!");
  ASSERT_EQ(numExpected, 1);
  ASSERT_EQ(res, 0);
}

// ensures user cannot specify invalid addresses
TEST_F(SDIBusControllerTest, refreshBadAddress) {
  int waitTime; int numExpected;
  int res = sdiBusPtr->refresh('\254', 0, &waitTime, &numExpected);
  ASSERT_NE(res, 0);
  ASSERT_EQ(SDIBusErrno, BAD_ADDRESS);
  ASSERT_EQ(Serial1.getOutputHistory().length(), 0);
}

// TODO(colin): figure out error for bad alternative number

// normal get data request
TEST_F(SDIBusControllerTest, getData) {
  // will send back 2 values
  Serial1.addToInputBuffer("0+3.72-4.132\r\n");

  int numExpected = 2;
  float buffer[16];
  int res = sdiBusPtr->getData('0', buffer, numExpected);
  ASSERT_EQ(res, 2);              // ensure response is equal to the number of values we get out
  ASSERT_EQ(buffer[0], 3.72f);
  ASSERT_EQ(buffer[1], -4.132f);
}
