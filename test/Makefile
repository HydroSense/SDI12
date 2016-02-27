CFLAGS += -I./ -I../

BINS := test test-device

all: $(BINS)

test: test.o SDIBusController.o SDIRemoteSensor.o ArduinoWrapper.o
	$(CXX) $(CFLAGS) -o $@ $^
test-device: test-device.o SDIBusController.o SDIRemoteSensor.o ArduinoWrapper.o
	$(CXX) $(CFLAGS) -o $@ $^

test-device.o: test-device.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<
test.o: test.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<
SDIBusController.o: ../SDIBusController.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<
SDIRemoteSensor.o: ../SDIRemoteSensor.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<
ArduinoWrapper.o: ArduinoWrapper.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(BINS) *.o *.socket
