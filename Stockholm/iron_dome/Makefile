TARGET = iron_dome
SRCS = main.cpp iron_dome.cpp daemon.cpp
HEADERS = iron_dome.hpp daemon.hpp
CXX = g++
CXXFLAGS = -std=c++17 -Wall
LIBS = -lcryptopp

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

clean:
	rm -f $(TARGET)
