CXX = g++
CXXFLAGS = -O2 -std=c++17 -Wall
LDLIBS = -lpthread -lm

all: Out.txt wavefunctions.dat diag_times.dat fig_waves.svg fig_diag_times.svg

# build Out.txt
Out.txt: main
	./main > Out.txt

# build wavefunction dataset
wavefunctions.dat: main
	./main -rmax 10 -dr 0.05 -wf wavefunctions.dat

# build diagonalization times dataset
diag_times.dat: main
	./main -diagf diag_times.dat

# plot wavefunctions
fig_waves.svg: wavefunctions.dat
	echo 'set terminal svg; \
	set output "$@"; \
	set xlabel "r"; set ylabel "|Psi|^2"; set tics in; \
	plot "$<" using 1:2 with linespoints title "k=0", \
	     "$<" using 1:3 with linespoints title "k=1", \
	     "$<" using 1:4 with linespoints title "k=2";' | gnuplot

# plot diagonalization times
fig_diag_times.svg: diag_times.dat
	echo 'set terminal svg; \
	set output "$@"; \
	set xlabel "Matrix size N"; set ylabel "Time (s)"; set tics in; \
	a=0.1; b=1; f(x)=a*x*x*x+b; \
	fit f(x) "$<" using 1:2 via a,b; \
	plot "$<" using 1:2 with linespoints title "times", f(x) title "Fit N^3";' | gnuplot

# build main
main: main.o matrix.o
	$(CXX) $(CXXFLAGS) -o main main.o matrix.o $(LDLIBS)

main.o: main.cc matrix.hpp
	$(CXX) $(CXXFLAGS) -c main.cc

matrix.o: matrix.cc matrix.hpp
	$(CXX) $(CXXFLAGS) -c matrix.cc

.PHONY: clean
clean:
	rm -f *.o main Out.txt wavefunctions.dat diag_times.dat out.rmax.data fig_*.svg