CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++20 -O2
LDLIBS   := -lm #Math library

# Targets

all: main

main: main.o sfuns.o
	$(CXX) $(LDLIBS) -o $@ $^

%.o: %.cc sfuns.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o main

run: main
	./main > Out.txt

.PHONY: all clean run