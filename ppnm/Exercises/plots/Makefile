CXX = g++
CXXFLAGS = -std=c++23 -Wall -O2

TARGET = main
SOURCES = main.cc functions.cc
OBJECTS = $(SOURCES:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(TARGET) *.o *.dat *.svg
