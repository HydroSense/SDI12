#include <Arduino.h>
#include <SDIBusController.hpp>

#include <gtest/gtest.h>

#define SERIAL_OUTPUT_PIN 10
#define FLOW_CONTROL_PIN 13
SDIBusController* sdiBusPtr;

class SDIBusControllerTest: public ::testing::Test {
public:
  SDIBusControllerTest() {
    pinMode(FLOW_CONTROL_PIN, 0);
    SDISerial *mySDISerial = new SDISerial(Serial1, SERIAL_OUTPUT_PIN, FLOW_CONTROL_PIN);
    sdiBusPtr = new SDIBusController(*mySDISerial);
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

TEST_F(SDIBusControllerTest, addressQueryAbsent) {
  // don't add anything to the input buffer

  char addr = 57;
  int res = sdiBusPtr->addressQuery(&addr);
  ASSERT_NE(res, 0);                          // we got a timeout error
  ASSERT_EQ(SDIBusErrno, TIMEOUT);
  ASSERT_EQ(addr, 57);                        // ensure function did not change address value
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
  ASSERT_STREQ(Serial1.getOutputHistory().c_str(), "0!");
}

TEST_F(SDIBusControllerTest, acknowledgeActiveBadAddress) {
  int res = sdiBusPtr->acknowledgeActive('\254');
  ASSERT_NE(res, 0);                                  // ensure non-ok return value
  ASSERT_EQ(SDIBusErrno, BAD_ADDRESS);                // ensure proper error number

  ASSERT_EQ(Serial1.getOutputHistory().length(), 0);  // ensure nothing written on serial
}

//Testing identify
TEST_F(SDIBusControllerTest, identifyNoOpt){
  //No optional
  Serial1.addToInputBuffer("a13HYDROSNS123456789\r\n");
  struct SDIDeviceIdentification devInfo;
  int res = sdiBusPtr->identify('a', &devInfo);

  ASSERT_EQ(res, 0);
  // cout << "ADDR CUSTOM: " << devInfo.addr << "\n";
  ASSERT_STREQ(devInfo.addr, "a");
  ASSERT_STREQ(devInfo.sdiVersion, "13");
  ASSERT_STREQ(devInfo.vendor, "HYDROSNS");
  ASSERT_STREQ(devInfo.modelNum, "123456");
  ASSERT_STREQ(devInfo.sensorVersion, "789");
  ASSERT_STREQ(Serial1.getOutputHistory().c_str(), "aI!");
}
TEST_F(SDIBusControllerTest, identifyPartialOpt){
  //Partial optional
  Serial1.addToInputBuffer("a13HYDROSNS123456789ThisIsPart\r\n");
  struct SDIDeviceIdentification devInfo;
  int res = sdiBusPtr->identify('a', &devInfo);

  ASSERT_EQ(res, 0);
  ASSERT_STREQ(devInfo.addr, "a");
  ASSERT_STREQ(devInfo.sdiVersion, "13");
  ASSERT_STREQ(devInfo.vendor, "HYDROSNS");
  ASSERT_STREQ(devInfo.modelNum, "123456");
  ASSERT_STREQ(devInfo.sensorVersion, "789");
  ASSERT_STREQ(devInfo.optional, "ThisIsPart");
  ASSERT_STREQ(Serial1.getOutputHistory().c_str(), "aI!");
}

TEST_F(SDIBusControllerTest, identifyFullOpt){
  //Full optional
  Serial1.addToInputBuffer("a13HYDROSNS123456789ThisIsFullTST\r\n");
  struct SDIDeviceIdentification devInfo;
  int res = sdiBusPtr->identify('a', &devInfo);

  ASSERT_EQ(res, 0);
  ASSERT_STREQ(devInfo.addr, "a");
  ASSERT_STREQ(devInfo.sdiVersion, "13");
  ASSERT_STREQ(devInfo.vendor, "HYDROSNS");
  ASSERT_STREQ(devInfo.modelNum, "123456");
  ASSERT_STREQ(devInfo.sensorVersion, "789");
  ASSERT_STREQ(devInfo.optional, "ThisIsFullTST");
  ASSERT_STREQ(Serial1.getOutputHistory().c_str(), "aI!");
}

TEST_F(SDIBusControllerTest, changeGoodAddress) {
  /* In this test, I assume I change the address from '0' to '1'
  */
  Serial1.addToInputBuffer("1\r\n");

  int res = sdiBusPtr->changeAddress('0', '1');
  ASSERT_EQ(res, 0); // ensure ok return value

  ASSERT_STREQ(Serial1.getOutputHistory().c_str(), "0A1!");
}

TEST_F(SDIBusControllerTest, changeBadAddress) {
  int res = sdiBusPtr->changeAddress('\254', '0');

  ASSERT_NE(res, 0);
  ASSERT_EQ(SDIBusErrno, BAD_ADDRESS);

  ASSERT_EQ(Serial1.getOutputHistory().length(), 0);
}

// TODO(colin): add identify commands

// typical start measurement command
TEST_F(SDIBusControllerTest, refresh) {
  Serial1.addToInputBuffer("a00101\r\n");

  int waitTime; int numExpected;
  int res = sdiBusPtr->refresh('a', 0, &waitTime, &numExpected);
  ASSERT_EQ(res, 0);
  ASSERT_EQ(waitTime, 1);
  ASSERT_EQ(numExpected, 1);

  ASSERT_STREQ(Serial1.getOutputHistory().c_str(), "aC!");
}

// alternative command
TEST_F(SDIBusControllerTest, altRefresh) {
  Serial1.addToInputBuffer("a01002\r\n");

  int waitTime; int numExpected;
  int res = sdiBusPtr->refresh('a', 2, &waitTime, &numExpected);
  ASSERT_EQ(res, 0);
  ASSERT_EQ(waitTime, 10);
  ASSERT_EQ(numExpected, 2);

  ASSERT_STREQ(Serial1.getOutputHistory().c_str(), "aC2!");
}

// instant refresh
TEST_F(SDIBusControllerTest, instRefresh) {
  Serial1.addToInputBuffer("300001\r\n");

  int waitTime; int numExpected;
  int res = sdiBusPtr->refresh('3', 0, &waitTime, &numExpected);
  ASSERT_EQ(res, 0);
  ASSERT_EQ(numExpected, 1);

  ASSERT_STREQ(Serial1.getOutputHistory().c_str(), "3C!");
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
  ASSERT_EQ(buffer[0], 3.72f);    // ensure we get the proper floating point values out
  ASSERT_EQ(buffer[1], -4.132f);

  ASSERT_STREQ(Serial1.getOutputHistory().c_str(), "0D0!");     // ensure we got everything out in a single go
}

TEST_F(SDIBusControllerTest, getDataMultipleQueries) {
  // will send back 2 values
  Serial1.addToInputBuffer("0+3.72\r\n0-4.132\r\n");

  int numExpected = 2;
  float buffer[16];
  int res = sdiBusPtr->getData('0', buffer, numExpected);
  ASSERT_EQ(res, 2);              // ensure response is equal to the number of values we get out
  ASSERT_EQ(buffer[0], 3.72f);    // ensure we get the proper floating point values out
  ASSERT_EQ(buffer[1], -4.132f);

  ASSERT_STREQ(Serial1.getOutputHistory().c_str(), "0D0!0D1!");     // ensure we got everything out in a single go
}

TEST_F(SDIBusControllerTest, getDataBadAddress) {
  Serial1.addToInputBuffer("1+3.72-4.132\r\n");

  int numExpected = 5;
  const float floatReference = -13.713;
  float buffer[16] = {floatReference};
  int res = sdiBusPtr->getData('\235', buffer, numExpected);
  ASSERT_NE(res, 0);                                                // ensure the response is not OK
  ASSERT_EQ(SDIBusErrno, BAD_ADDRESS);                              // ensure we are getting a BAD_ADDRESS error
  for (int i=0; i < 16; i++) {
    ASSERT_EQ(buffer[0], floatReference);                           // ensure every value in the initialized buffer didn't get changed
  }
  ASSERT_EQ(Serial1.getOutputHistory().length(), 0);                // ensure we didn't write anything to the buffer
}

// TODO(colin): add timeouts to the getData function
