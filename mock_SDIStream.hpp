#ifndef MOCK_SDISTREAM
#define MOCK_SDISTREAM

#include <gmock/gmock.h>

#include "SDIStream.hpp"

class MockSDIStream : public SDIStream {
public:
  MOCK_METHOD0(sendPreamble, void());
  MOCK_METHOD0(setBufferRead, void());
  MOCK_METHOD0(setBufferWrite, void());

  MOCK_METHOD0(available, int());
  MOCK_METHOD0(read, int());
  MOCK_METHOD0(peek, int());
  MOCK_METHOD0(flush, void());

  MOCK_METHOD1(write, int(char));
  MOCK_METHOD1(write, int(char*));

  MOCK_METHOD0(begin, void());
  MOCK_METHOD0(end, void());
};

#endif
