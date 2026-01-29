CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++20 -O2
LDLIBS   := -lm #Math library

# Targets

all: main

# Build the final executable
main: main.o sfuns.o
	$(CXX) $(LDLIBS) -o $@ $^

# Compile each .cc file into an object file
%.o: %.cc sfuns.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Convenience: rebuild everything from scratch
clean:
	rm -f *.o main

# Run the program and capture its output
run: main
	./main > Out.txt

.PHONY: all clean run