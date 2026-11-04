CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

TARGET = echoServ

all: $(TARGET)

$(TARGET): echoServ.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
