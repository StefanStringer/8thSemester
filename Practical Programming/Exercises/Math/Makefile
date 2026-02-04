CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++20 -O2
LDLIBS   := -lm

all: main

main: main.o sfuns.o
	$(CXX) -o $@ $^ $(LDLIBS)

%.o: %.cpp sfuns.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.cc sfuns.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o main

run: main
	./main > Out.txt

.PHONY: all clean run
