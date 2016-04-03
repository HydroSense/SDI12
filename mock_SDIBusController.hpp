#include <gmock/gmock.h>
#include "SDIBusController.hpp"

class MockSDIBusController : public SDIBusController {
public:
  MockSDIBusController() {};

  MOCK_METHOD0(begin, void());
  MOCK_METHOD0(end, void());

  MOCK_METHOD1(addressQuery, int(char* outAddr));
  MOCK_METHOD1(acknowledgeActive, int(char addr));
  MOCK_METHOD2(identify, int(char addr, struct SDIDeviceInformation* devInfo));

  MOCK_METHOD4(refresh, int(char addr, int altno, int* waitTime, int* numExpected));
  MOCK_METHOD3(getData, int(char addr, float* buffer, int numExpected));
  MOCK_METHOD2(changeAddress, int(char oldAddr, char newAddr));

  MOCK_METHOD1(respondToAcknowledgeActive, int(char addr));

  MOCK_METHOD1(respondToSendIdentificationNoOpt, int(char addr));
  MOCK_METHOD1(respondToSendIdentificationPartialOpt, int(char addr));
  MOCK_METHOD1(respondToSendIdentificationFullOpt, int(char addr));

  MOCK_METHOD1(respondToChangeAddress, int(char addr));
  MOCK_METHOD1(respondToAddressQuery, int(char addr));
  MOCK_METHOD2(respondToRefresh, int(char addr, int altno));
}
