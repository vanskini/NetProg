CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

TARGET = time_cli

all: $(TARGET)

$(TARGET): time_cli.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
