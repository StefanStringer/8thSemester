CXX = g++
CXXFLAGS = -O2 -std=c++23 -pthread
TARGET = main
SRC = main.cc

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)
