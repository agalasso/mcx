ALL := hello

all: $(ALL)

clean:
	rm -f $(ALL) *.o

hello.o: hello.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $^

crc16.o: crc16.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $^

hello: hello.o crc16.o
	$(CXX) $(LDFLAGS) -o $@ $^
