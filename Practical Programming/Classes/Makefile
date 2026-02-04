CXX = g++ -std=c++23
CXXFLAGS = -Wall -Wextra -O
LDLIBS += -lstdc++ -lm

Out.txt : main
#	./main > Out.txt //bad method
	./$< > $@ //best method to do

main: main.o hello.O
main.o hello.h