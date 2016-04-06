#include <Arduino.h>
#include <SDIBusController.hpp>
#include <mock_SDIStream.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::InSequence;
using ::testing::Return;
using ::testing::AtLeast;
using ::testing::_;
using ::testing::Matcher;
using ::testing::DefaultValue;
using ::testing::AnyNumber;

#define SERIAL_OUTPUT_PIN 10
#define FLOW_CONTROL_PIN 13

class SDIBusControllerTest: public ::testing::Test {
protected:
  SDIBusController* sdiBusPtr;
  ArduinoMock& mockArduino;
  MockSDIStream mockSDIStream;
public:
  SDIBusControllerTest():
    mockArduino(*arduinoMockInstance()){
  }

  ~SDIBusControllerTest() {
    releaseArduinoMock();
  }

  virtual void SetUp() {
    EXPECT_CALL(mockSDIStream, setBufferRead());
    sdiBusPtr = new SDIBusController(mockSDIStream);
  }

  virtual void TearDown() {
  }

  void transactionSequence(const char* out, const char* in) {
    EXPECT_CALL(mockArduino, delay(_))
      .Times(AnyNumber());

    {
      InSequence sequence;

      EXPECT_CALL(mockSDIStream, setBufferWrite());
      EXPECT_CALL(mockSDIStream, sendPreamble());

      for (int i=0; out[i] != '\0'; i++) {
        EXPECT_CALL(mockSDIStream, write(out[i]));
      }

      EXPECT_CALL(mockSDIStream, flush());
      EXPECT_CALL(mockSDIStream, setBufferRead());

      // if there is no input string, then available will return 0
      if (strlen(in) == 0) {
        EXPECT_CALL(mockSDIStream, available())
          .WillRepeatedly(Return(0));
      }
      for (int i=0; in[i] != '\0'; i++) {
        EXPECT_CALL(mockSDIStream, available())
          .WillRepeatedly(Return(strlen(in)-i));

        EXPECT_CALL(mockSDIStream, read())
          .WillOnce(Return(in[i]));
      }
    }
  }

  void noTransactionSequence() {
    EXPECT_CALL(mockArduino, delay(_))
      .Times(0);
    EXPECT_CALL(mockSDIStream, setBufferWrite())
      .Times(0);
    EXPECT_CALL(mockSDIStream, setBufferRead())
      .Times(0);

    EXPECT_CALL(mockSDIStream, read())
      .Times(0);
    EXPECT_CALL(mockSDIStream, write(Matcher<char>(_)))
      .Times(0);
    EXPECT_CALL(mockSDIStream, write(Matcher<char*>(_)))
      .Times(0);
    EXPECT_CALL(mockSDIStream, flush())
      .Times(0);
  }
};


TEST_F(SDIBusControllerTest, addressQuery) {
  transactionSequence("?!", "1\r\n");

  char addr;
  int res = sdiBusPtr->addressQuery(&addr);
  ASSERT_EQ(res, 0);
  ASSERT_EQ(addr, '1');

  transactionSequence("?!", "x\r\n");

  res = sdiBusPtr->addressQuery(&addr);
  ASSERT_EQ(res, 0);
  ASSERT_EQ(addr, 'x');
}

TEST_F(SDIBusControllerTest, addressQueryAbsent) {
  // don't add anything to the input buffer
  transactionSequence("?!", "");

  char addr = 57;
  int res = sdiBusPtr->addressQuery(&addr);
  ASSERT_NE(res, 0);                          // we got a timeout error
  ASSERT_EQ(SDIBusErrno, TIMEOUT);
  ASSERT_EQ(addr, 57);                        // ensure function did not change address value
}

TEST_F(SDIBusControllerTest, acknowledgeActivePresent) {
  transactionSequence("0!", "0\r\n");

  int res = sdiBusPtr->acknowledgeActive('0');
  ASSERT_EQ(res, 0);
}

TEST_F(SDIBusControllerTest, acknowledgeActiveAbsent) {
  transactionSequence("0!", "");

  int res = sdiBusPtr->acknowledgeActive('0');
  ASSERT_NE(res, 0);
  ASSERT_EQ(SDIBusErrno, TIMEOUT);
}

TEST_F(SDIBusControllerTest, acknowledgeActiveBadAddress) {
  noTransactionSequence();

  int res = sdiBusPtr->acknowledgeActive('\254');
  ASSERT_NE(res, 0);                                  // ensure non-ok return value
  ASSERT_EQ(SDIBusErrno, BAD_ADDRESS);                // ensure proper error number
}

/*
//Testing identify
TEST_F(SDIBusControllerTest, identifyNoOpt){
  transactionSequence("aI!", "a13HYDROSNS123456789\r\n");

  struct SDIDeviceIdentification devInfo;
  int res = sdiBusPtr->identify('a', &devInfo);

  ASSERT_EQ(res, 0);
  // cout << "ADDR CUSTOM: " << devInfo.addr << "\n";
  ASSERT_STREQ(devInfo.addr, "a");
  ASSERT_STREQ(devInfo.sdiVersion, "13");
  ASSERT_STREQ(devInfo.vendor, "HYDROSNS");
  ASSERT_STREQ(devInfo.modelNum, "123456");
  ASSERT_STREQ(devInfo.sensorVersion, "789");
}
*/

/*
TEST_F(SDIBusControllerTest, identifyPartialOpt){
  //Partial optional
  Serial1.transactionSequence("a13HYDROSNS123456789ThisIsPart\r\n");
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
  Serial1.transactionSequence("a13HYDROSNS123456789ThisIsFullTST\r\n");
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
*/


TEST_F(SDIBusControllerTest, changeGoodAddress) {
  //In this test, I assume I change the address from '0' to '1'
  transactionSequence("0A1!", "1\r\n");

  int res = sdiBusPtr->changeAddress('0', '1');
  ASSERT_EQ(res, 0); // ensure ok return value
}

TEST_F(SDIBusControllerTest, changeBadAddress) {
  noTransactionSequence();

  int res = sdiBusPtr->changeAddress('\254', '0');

  ASSERT_NE(res, 0);
  ASSERT_EQ(SDIBusErrno, BAD_ADDRESS);
}

// typical start measurement command
TEST_F(SDIBusControllerTest, refresh) {
  transactionSequence("aC!", "a00101\r\n");

  int waitTime; int numExpected;
  int res = sdiBusPtr->refresh('a', 0, &waitTime, &numExpected);
  ASSERT_EQ(res, 0);
  ASSERT_EQ(waitTime, 1);
  ASSERT_EQ(numExpected, 1);
}

// alternative command
TEST_F(SDIBusControllerTest, altRefresh) {
  transactionSequence("aC2!", "a01002\r\n");

  int waitTime; int numExpected;
  int res = sdiBusPtr->refresh('a', 2, &waitTime, &numExpected);
  ASSERT_EQ(res, 0);
  ASSERT_EQ(waitTime, 10);
  ASSERT_EQ(numExpected, 2);
}

// instant refresh
TEST_F(SDIBusControllerTest, instRefresh) {
  transactionSequence("3C!", "300001\r\n");

  int waitTime; int numExpected;
  int res = sdiBusPtr->refresh('3', 0, &waitTime, &numExpected);
  ASSERT_EQ(res, 0);
  ASSERT_EQ(numExpected, 1);
}

// ensures user cannot specify invalid addresses
TEST_F(SDIBusControllerTest, refreshBadAddress) {
  noTransactionSequence();

  int waitTime; int numExpected;
  int res = sdiBusPtr->refresh('\254', 0, &waitTime, &numExpected);
  ASSERT_NE(res, 0);
  ASSERT_EQ(SDIBusErrno, BAD_ADDRESS);
}

// normal get data request
TEST_F(SDIBusControllerTest, getData) {
  // will send back 2 values
  transactionSequence("0D0!", "0+3.72-4.132\r\n");

  int numExpected = 2;
  float buffer[16];
  int res = sdiBusPtr->getData('0', buffer, numExpected);
  ASSERT_EQ(res, 2);              // ensure response is equal to the number of values we get out
  ASSERT_EQ(buffer[0], 3.72f);    // ensure we get the proper floating point values out
  ASSERT_EQ(buffer[1], -4.132f);
}

// TODO(Colby/Matt): ensure this test passes
//TEST_F(SDIBusControllerTest, getDataBadResponse) {
  // will send back 2 values
//  transactionSequence("aD0!", "0+3.72-4.132\r\n");
//}

TEST_F(SDIBusControllerTest, getDataMultipleQueries) {
  // will send back 2 values
  {
    InSequence sequence;
    transactionSequence("aD0!", "a+3.72\r\n");
    transactionSequence("aD1!", "a-4.132\r\n");
  }

  int numExpected = 2;
  float buffer[16];
  int res = sdiBusPtr->getData('a', buffer, numExpected);
  ASSERT_EQ(res, 2);              // ensure response is equal to the number of values we get out
  ASSERT_EQ(buffer[0], 3.72f);    // ensure we get the proper floating point values out
  ASSERT_EQ(buffer[1], -4.132f);
}

TEST_F(SDIBusControllerTest, getDataBadAddress) {
  noTransactionSequence();

  int numExpected = 5;
  const float floatReference = -13.713;
  float buffer[16] = {floatReference};
  int res = sdiBusPtr->getData('\235', buffer, numExpected);

  ASSERT_NE(res, 0);                                                // ensure the response is not OK
  ASSERT_EQ(SDIBusErrno, BAD_ADDRESS);                              // ensure we are getting a BAD_ADDRESS error
  for (int i=0; i < 16; i++) {
    ASSERT_EQ(buffer[0], floatReference);                           // ensure every value in the initialized buffer didn't get changed
  }
}
