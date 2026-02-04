#CXX = g++ -std=c++23
CXX = clang++ -std=c++23
CXXFLAGS = -Wall -Wextra -Werror -O
LDLIBS += -lstdc++ -lm

all: Out.txt
	cat $<
Out.txt : main
#	./main > Out.txt //bad method
	./$< > $@ 

main: main.o hello.O
main.o hello.h