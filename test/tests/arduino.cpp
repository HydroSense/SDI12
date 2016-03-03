#include <Arduino.h>

#include <gtest/gtest.h>

class ArduinoEnvironmentTest: public ::testing::Test {
public:
  ArduinoEnvironmentTest() {

  }

  virtual void SetUp() {
    Environment.reset();
  }

  virtual void TearDown() {
  }
};

TEST_F(ArduinoEnvironmentTest, pinMode) {
  int pin = 5;
  int otherPin = 7;

  ASSERT_EQ(Environment.pinTristates[pin], PIN_STATE_UNDEF);
  ASSERT_EQ(Environment.pinTristates[otherPin], PIN_STATE_UNDEF);

  pinMode(pin, INPUT);
  pinMode(otherPin, OUTPUT);
  ASSERT_EQ(Environment.pinTristates[pin], IN);
  ASSERT_EQ(Environment.pinTristates[otherPin], OUT);

  pinMode(pin, OUTPUT);
  pinMode(otherPin, INPUT);
  ASSERT_EQ(Environment.pinTristates[pin], OUT);
  ASSERT_EQ(Environment.pinTristates[otherPin], IN);
}

TEST_F(ArduinoEnvironmentTest, digitalWrite) {
  int outputPin = 5;
  int inputPin = 7;

  pinMode(outputPin, OUTPUT);
  pinMode(inputPin, INPUT);

  digitalWrite(outputPin, 1);
  ASSERT_EQ(Environment.pinSignalStates[outputPin], 1);
  digitalWrite(outputPin, 0);
  ASSERT_EQ(Environment.pinSignalStates[outputPin], 0);

  // make sure a pin mode exception is thrown
  bool caught = false;
  try {
    digitalWrite(inputPin, 1);
  } catch(PinModeException& e) {
    caught = true;
  }

  ASSERT_TRUE(caught);
}

TEST_F(ArduinoEnvironmentTest, timeInitialization) {
  ASSERT_EQ(Environment.timeUsec, 0L);
}

TEST_F(ArduinoEnvironmentTest, delayMicroseconds) {
  delayMicroseconds(1000);

  ASSERT_GE(Environment.timeUsec, 1000L);
  ASSERT_LT(Environment.timeUsec, 1500L);
}

TEST_F(ArduinoEnvironmentTest, delay) {
  delay(250);

  ASSERT_GE(Environment.timeUsec, 1000L*250L);
  ASSERT_LT(Environment.timeUsec, 1000L*400L);
}
