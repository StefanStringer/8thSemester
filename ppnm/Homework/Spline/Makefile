CXX=g++
CXXFLAGS=-O2 -std=c++20 -Wall

SOURCES=main.cc spline.cc
OBJECTS=$(SOURCES:.cc=.o)
EXEC=main

all: Out.txt fig_linear.svg fig_quadratic.svg fig_cubic.svg fig_total.svg

%.o: %.cc spline.hpp matrix.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

Out.txt: $(EXEC)
	./$(EXEC) > $@

fig_linear.svg: linear.dat
	echo 'set terminal svg; \
	set output "$@"; \
	set xlabel "x"; \
	set ylabel "cos(x)"; \
	plot "$<" using 1:2 with lines title "linear spline";' | gnuplot

fig_quadratic.svg: quadratic.dat
	echo 'set terminal svg; \
	set output "$@"; \
	set xlabel "x"; \
	set ylabel "sin(x)"; \
	plot "$<" using 1:2 with lines title "quadratic spline";' | gnuplot

fig_cubic.svg: cubic.dat
	echo 'set terminal svg; \
	set output "$@"; \
	set xlabel "x"; \
	set ylabel "sin(x)"; \
	plot "$<" using 1:2 with lines title "cubic spline";' | gnuplot


fig_total.svg: linear.dat quadratic.dat cubic.dat
	echo 'set terminal svg; \
	set output "$@"; \
	set xlabel "x"; \
	set ylabel "f(x)"; \
	set grid; \
	plot \
	"linear.dat" using 1:2 with lines lw 2 title "linear spline", \
	"quadratic.dat" using 1:2 with lines lw 2 title "quadratic spline", \
	"cubic.dat" using 1:2 with lines lw 2 title "cubic spline", \
	sin(x) with lines lw 2 dt 2 title "sin(x)" ' | gnuplot

clean:
	rm -f *.o $(EXEC) Out.txt *.dat *.svg