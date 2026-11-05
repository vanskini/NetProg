CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

TARGET = EchoCli

all: $(TARGET)

$(TARGET): EchoCli.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
