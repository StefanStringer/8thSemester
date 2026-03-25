CXX = g++
CXXFLAGS = -O2 -std=c++20 -Wall

SRC = main.cc ode.cpp matrix.cpp
OBJ = $(SRC:.cpp=.o)

EXEC = main

all: Out.txt fig_oscillator.svg

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

Out.txt: $(EXEC)
	./$(EXEC) > Out.txt

fig_oscillator.svg: oscillator.dat
	echo 'set terminal svg size 800,600; \
	set output "$@"; \
	set xlabel "x"; \
	set ylabel "u(x)"; \
	set grid; \
	plot \
	"$<" using 1:2 with lines title "u(x) numerical", \
	"$<" using 1:4 with lines title "cos(x) exact";' | gnuplot

clean:
	rm -f *.o $(EXEC) *.dat *.svg Out.txt