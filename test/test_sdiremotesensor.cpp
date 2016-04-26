#include <Arduino.h>
#include <SDIRemoteSensor.hpp>
#include <mock_SDIStream.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Return;
using ::testing::InSequence;
using ::testing::AnyNumber;
using ::testing::AtLeast;
using ::testing::_;

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

  void transactionSequence(const char *out, const char *in){
      /* This is really frustrating it's not working. The sequence should be:
      1 - available()
      1 - peek()
      1 - available()
      strlen(in) - read()
      */
      EXPECT_CALL(mockArduino, delay(_))
      .Times(AnyNumber());
      printf("in: %s, strlen(in): %d\n", in, strlen(in));

        {
            InSequence sequence;

            EXPECT_CALL(mockSDIStream, available())
            .WillRepeatedly(Return(strlen(in)));

            EXPECT_CALL(mockSDIStream, peek())
            .WillOnce(Return('!')); // last character is always '!'

            EXPECT_CALL(mockSDIStream, available())
            .WillRepeatedly(Return(strlen(in)));

            for(int i=0; in[i] != '\0'; i++){
                EXPECT_CALL(mockSDIStream, read())
                .WillOnce(Return(in[i]));
            }


        }

  }
};

SDIResponse dummyStartMeasurementHandler(){
  // Made up data.
  char *data = new char;
  // response: atttn<CR><LF>
  data = (char *)"a0013";
  return data;
//  return SDIResponse::OK;
}

SDIResponse dummyGetDataHandler(){
    char *data = new char;
    // response: a<values><CR><LF>
    data = (char *) "a+150-60+25";
    return data;
}

TEST_F(SDIRemoteSensorTest, listenStartMeasurementTest){
  sensorPtr->registerStartMeasurementHandler(dummyStartMeasurementHandler);
    /*
    EXPECT_CALL(mockSDIStream, available()).WillRepeatedly(Return(3));
    EXPECT_CALL(mockSDIStream, peek()).WillOnce(Return('!'));
    EXPECT_CALL(mockSDIStream, read()).WillOnce(Return(str[0]))
    .WillOnce(Return(str[1]))
    .WillOnce(Return(str[2]));
    */
    char *fromController = (char *) "aC!";
    transactionSequence("", fromController);
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
  struct SDIDeviceIdentification readID = sensorPtr->getMySDIDeviceIdentification();
  ASSERT_STREQ(readID.addr, "a");
  ASSERT_STREQ(readID.sdiVersion, "13");
  ASSERT_STREQ(readID.vendor, "HYDROSNS");
  ASSERT_STREQ(readID.modelNum, "123456");
  ASSERT_STREQ(readID.sensorVersion, "789");
  ASSERT_STREQ(readID.optional, "");
}

TEST_F(SDIRemoteSensorTest, registerStartMeasurementHandler){
  int result = sensorPtr->registerStartMeasurementHandler(dummyStartMeasurementHandler);
  ASSERT_EQ(result, 0);
}

TEST_F(SDIRemoteSensorTest, listenGetDataTest){
    sensorPtr->registerStartMeasurementHandler(dummyStartMeasurementHandler);
    sensorPtr->registerGetDataHandler(dummyGetDataHandler);


}

/*
TEST_F(SDIRemoteSensorTest, querySensorAddress) {
  char *addr;
  EXPECT_CALL(*mockBusControllerPtr, addressQuery)
    .WillOnce(DoAll(SetArg2Return(0));
}
*/
