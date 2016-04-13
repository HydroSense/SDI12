#include <Arduino.h>
#include <SDIRemoteSensor.hpp>
#include <mock_SDIStream.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class SDIRemoteSensorTest : public ::testing::Test {
protected:
  SDIRemoteSensor* sensorPtr;
  ArduinoMock &mockArduino;
  MockSDIStream mockSDIStream;
  char address = 'a';

public:
  /*
  SDIRemoteSensorTest() {
    char address = 'a';
    mockBusControllerPtr = new MockSDIBusController();
    sensorPtr = new SDIRemoteSensor(*mockBusControllerPtr, address);
  }
  */
  SDIRemoteSensorTest():
    mockArduino(*arduinoMockInstance()){
    }

  ~SDIRemoteSensorTest() {
    releaseArduinoMock();
  }

  virtual void SetUp() {
    sensorPtr = new SDIRemoteSensor(mockSDIStream, address);
  }

  virtual void TearDown() {
  }
};

TEST_F(SDIRemoteSensorTest, listen) {
  int result = sensorPtr->listen();

  ASSERT_EQ(result, 0);
}

TEST_F(SDIRemoteSensorTest, setIdentificationNoOpt){
  struct SDIDeviceIdentification myID;
  strcpy(myID.addr, "a");
  strcpy(myID.sdiVersion, "13");
  strcpy(myID.vendor, "HYDROSNS");
  strcpy(myID.modelNum, "123456");
  strcpy(myID.sensorVersion, "789");
  strcpy(myID.optional, "");

  int result = sensorPtr->setIdentification(myID);
//  ASSERT_STREQ(sensorPtr->)
}

/*
TEST_F(SDIRemoteSensorTest, querySensorAddress) {
  char *addr;
  EXPECT_CALL(*mockBusControllerPtr, addressQuery)
    .WillOnce(DoAll(SetArg2Return(0));
}
*/
