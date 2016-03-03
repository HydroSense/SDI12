#include <stdio.h>
#include <iostream>
using namespace std;

#include <Arduino.h>
#include <SDIBusController.hpp>
#include <SDIRemoteSensor.hpp>

#define FLOW_CONTROL_PIN 13

#include <gtest/gtest.h>

class MasterTest: public ::testing::Test {
public:
  MasterTest() {

  }

  virtual void SetUp() {
    // reset the environment every time
    Environment.reset();
  }

  virtual void TearDown() {
  }
};

TEST_F(MasterTest, t) {
}
