CFLAGS += -I./ -I../

all: test

test: test.o SDIBusController.o SDIRemoteSensor.o ArduinoWrapper.o
	$(CXX) $(CFLAGS) -o $@ $^

test.o: test.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<
SDIBusController.o: ../SDIBusController.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<
SDIRemoteSensor.o: ../SDIRemoteSensor.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<
ArduinoWrapper.o: ArduinoWrapper.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	rm -f test *.o
