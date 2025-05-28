TARGET = ransom
SRCS = main.cpp crypto.cpp
HEADERS = crypto.hpp
CXX = g++
CXXFLAGS = -std=c++17 -Wall
LIBS = -lcryptopp

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

clean:
	rm -f $(TARGET)
