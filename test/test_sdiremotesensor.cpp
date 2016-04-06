/*

#include <Arduino.h>
#include <SDIBusController.hpp>
#include <SDIRemoteSensor.hpp>

#include <gtest/gtest.h>
class SDIRemoteSensorTest : public ::testing::Test {
private:
  MockSDIBusController* mockBusControllerPtr;
  SDIRemoteSensor* sensorPtr;
  char address;

public:
  SDIRemoteSensorTest() {
    address = 'a';
    mockBusControllerPtr = new MockSDIBusController();
    sensorPtr = new SDIRemoteSensor(*mockBusControllerPtr, address);
  }

  ~SDIRemoteSensorTest() {
    delete sensorPtr;
    delete MockSDIBusControllerPtr;
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
}

TEST_F(SDIRemoteSensorTest, busyAtStart) {
  int busy = sensorPtr->busy();

  ASSERT_EQ(busy, 0);
}
*/

/*
TEST_F(SDIRemoteSensorTest, querySensorAddress) {
  char *addr;
  EXPECT_CALL(*mockBusControllerPtr, addressQuery)
    .WillOnce(DoAll(SetArg2Return(0));
}
*/
