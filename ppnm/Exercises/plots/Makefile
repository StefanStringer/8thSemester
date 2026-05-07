CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -O2

TARGET = main
SOURCES = main.cc functions.cc
OBJECTS = $(SOURCES:.cc=.o)

all: $(TARGET) run

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

run: $(TARGET)
	@echo "Running program to generate plots..."
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o *.dat *.svg

.PHONY: all clean run