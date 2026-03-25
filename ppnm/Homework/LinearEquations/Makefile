CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++23 -O2

SRC = main.cc matrix.cpp qr.cpp
OBJ = $(SRC:.cpp=.o)

all: Out.txt fig_QR_timing.svg

main: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

Out.txt: main
	./main > Out.txt

out.times.data: main
	>$@
	for N in $$(seq 100 20 400); do \
		time --format "$$N %e" --output $@ --append \
		./main -size $$N 1>out 2>err ;\
	done

fig_QR_timing.svg: out.times.data
	echo 'set terminal svg size 800,600; \
	set output "$@"; \
	set xlabel "Matrix size N"; \
	set ylabel "Time (s)"; \
	set grid; \
	a=1e-6; \
	f(x)=a*x**3; \
	fit f(x) "$<" using 1:2 via a; \
	plot "$<" using 1:2 with points pt 7 title "Measured time", \
	     f(x) with lines lw 2 title "Fit ~ N^3";' | gnuplot

clean:
	rm -f main *.o Out.txt out.times.data fig_QR_timing.svg