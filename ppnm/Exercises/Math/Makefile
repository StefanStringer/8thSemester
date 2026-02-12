CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++23 -O2
LDLIBS = -lm

# run: main
# 	./main $(ARGS)

Out.txt: main
	./main > Out.txt

main: main.o sfuns.o
	$(CXX) $^ $(LDLIBS) -o main

main.o: main.cc sfuns.h
	$(CXX) $(CXXFLAGS) -c main.cc

sfuns.o: sfuns.cc sfuns.h
	$(CXX) $(CXXFLAGS) -c sfuns.cc

.PHONY: clean
clean:
	$(RM) -f main main.o sfuns.o Out.txt

test:
	@echo $(CXX)
	@echo $(CXXFLAGS)
	@echo $(RM)