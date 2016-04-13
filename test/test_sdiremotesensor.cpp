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
  }

  virtual void TearDown() {
  }
};

TEST_F(SDIRemoteSensorTest, listenTest) {
  int result = sensorPtr->listen();

  ASSERT_EQ(result, 0);
}

/*
TEST_F(SDIRemoteSensorTest, querySensorAddress) {
  char *addr;
  EXPECT_CALL(*mockBusControllerPtr, addressQuery)
    .WillOnce(DoAll(SetArg2Return(0));
}
*/
