CXX=g++
CXXFLAGS=-O2 -std=c++17 -Wall
SOURCES=main.cc
OBJECTS=$(SOURCES:.cc=.o)
EXEC=main

# all targets
all: Out.txt wavefunctions.dat diag_times.dat fig_waves.svg fig_diag_times.svg

# compile object
%.o: %.cc matrix.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# link executable
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

# run main and save standard output
Out.txt: $(EXEC)
	./$(EXEC) > $@

# build wavefunction dataset
wavefunctions.dat: $(EXEC)
	./$(EXEC) -rmax 10 -dr 0.05 -wf $@

# build diagonalization times dataset
diag_times.dat: $(EXEC)
	> $@
	for N in 20 40 60 80 100 120 140 160 180 200; do \
		./$(EXEC) -diagf tmp.$$N $$N > tmp.$$N.out 2>&1 & \
	done
	wait
	for N in 20 40 60 80 100 120 140 160 180 200; do \
		cat tmp.$$N >> $@; \
		rm -f tmp.$$N tmp.$$N.out; \
	done

# plot wavefunctions
fig_waves.svg: wavefunctions.dat
	echo 'set terminal svg; \
	set output "$@"; \
	set tics in; \
	set xlabel "r"; set ylabel "|Psi|^2"; \
	plot \
		"$<" using 1:2 with linespoints title "k=0", \
		"$<" using 1:3 with linespoints title "k=1", \
		"$<" using 1:4 with linespoints title "k=2";' | gnuplot

# plot diagonalization times
fig_diag_times.svg: diag_times.dat
	echo 'set terminal svg; \
	set output "$@"; \
	set tics in; \
	set xlabel "Matrix size N"; set ylabel "Time (s)"; \
	a=0.1; b=1; f(x)=a*x*x*x+b; \
	fit f(x) "$<" using 1:2 via a,b; \
	plot "$<" using 1:2 with linespoints title "times", f(x) title "Fit N^3";' | gnuplot

clean:
	rm -f *.o $(EXEC) Out.txt wavefunctions.dat diag_times.dat fig_waves.svg fig_diag_times.svg