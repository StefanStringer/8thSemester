CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++23 -O2

# Add ALL source files here
SRC = main.cc matrix.cpp qr.cpp
OBJ = $(SRC:.cpp=.o)

all: Out.txt

main: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

Out.txt: main
	./main > Out.txt

clean:
	rm -f main *.o Out.txt