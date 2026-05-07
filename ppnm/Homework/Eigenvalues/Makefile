# Compiler and Flags
CXX = g++
CXXFLAGS = -O2 -std=c++17 -Wall -Wextra
LDFLAGS = 

# Files
SOURCES = main.cc
OBJECTS = $(SOURCES:.cc=.o)
EXEC = main

.PHONY: all clean

all: Out.txt wavefunctions.dat diag_times.dat convergence_rmax.dat convergence_dr.dat \
     fig_waves.svg fig_scaling.svg fig_conv_rmax.svg fig_conv_dr.svg

# Compilation
%.o: %.cc matrix.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

# --- PART 1: Basic Run & Wavefunctions ---

Out.txt: $(EXEC)
	./$(EXEC) -rmax 10 -dr 0.3 > $@

wavefunctions.dat: $(EXEC)
	./$(EXEC) -rmax 10 -dr 0.05 -wf $@ -e0only

# --- PART 2: Convergence Analysis ---

convergence_rmax.dat: $(EXEC)
	> $@
	@echo "Running rmax convergence (dr=0.1)..."
	@for rmax in 5 8 10 12 15 20; do \
		echo "  Testing rmax=$$rmax..."; \
		./$(EXEC) -rmax $$rmax -dr 0.1 -e0only 2>/dev/null | grep "E\[0\]" | awk '{print $$3}' | xargs -I {} echo "$$rmax {}" >> $@; \
	done
	@echo "Done rmax convergence."

convergence_dr.dat: $(EXEC)
	> $@
	@echo "Running dr convergence (rmax=15)..."
	@for dr in 0.5 0.3 0.2 0.1 0.05 0.02; do \
		echo "  Testing dr=$$dr..."; \
		./$(EXEC) -rmax 15 -dr $$dr -e0only 2>/dev/null | grep "E\[0\]" | awk '{print $$3}' | xargs -I {} echo "$$dr {}" >> $@; \
	done
	@echo "Done dr convergence."

# --- PART 3: Scaling & Timing (Fixed Parallel Logic) ---

diag_times.dat: $(EXEC)
	> $@
	@echo "Running scaling tests (parallel)..."
	@rm -f tmp.*
	@for N in 20 40 60 80 100 120 140 160 180 200; do \
		./$(EXEC) -diagf tmp.$$N $$N > /dev/null 2>&1 & \
	done
	@echo "Waiting for jobs to finish..."
	@wait
	@echo "Collecting results..."
	@for N in 20 40 60 80 100 120 140 160 180 200; do \
		if [ -f tmp.$$N ]; then \
			cat tmp.$$N >> $@; \
			rm -f tmp.$$N; \
		else \
			echo "ERROR: tmp.$$N missing. Retrying sequentially..." >&2; \
			./$(EXEC) -diagf tmp.$$N $$N >> $@; \
			rm -f tmp.$$N; \
		fi; \
	done
	@echo "Done scaling tests."

# --- PLOTTING (Fixed Gnuplot Syntax) ---

fig_waves.svg: wavefunctions.dat
	@echo "Plotting wavefunctions..."
	@echo 'set terminal svg size 800,600; \
	set output "fig_waves.svg"; \
	set title "Hydrogen s-wave Radial Probability Density"; \
	set xlabel "r (Bohr)"; set ylabel "|f(r)|^2"; \
	set grid; \
	plot "wavefunctions.dat" using 1:2 with linespoints title "k=0", \
	     "wavefunctions.dat" using 1:3 with linespoints title "k=1", \
	     "wavefunctions.dat" using 1:4 with linespoints title "k=2";' | gnuplot

fig_scaling.svg: diag_times.dat
	@echo "Plotting scaling (N^3)..."
	@echo 'set terminal svg size 800,600; \
	set output "fig_scaling.svg"; \
	set title "Jacobi Diagonalization Scaling"; \
	set xlabel "Matrix Size N"; set ylabel "Time (s)"; \
	set logscale xy; \
	a=1e-9; f(x)=a*x**3; \
	fit f(x) "diag_times.dat" using 1:2 via a; \
	plot "diag_times.dat" using 1:2 with points pt 7 title "Measured", \
	     f(x) with lines lw 2 title "Fit ~ N^3";' | gnuplot

fig_conv_rmax.svg: convergence_rmax.dat
	@echo "Plotting rmax convergence..."
	@echo 'set terminal svg size 800,600; \
	set output "fig_conv_rmax.svg"; \
	set title "Convergence w.r.t rmax (dr=0.1)"; \
	set xlabel "rmax (Bohr)"; set ylabel "Ground State Energy (Hartree)"; \
	set grid; \
	set yrange [-0.6:-0.4]; \
	plot "convergence_rmax.dat" using 1:2 with linespoints title "E_0", \
	     -0.5 with lines title "Exact (-0.5)";' | gnuplot

fig_conv_dr.svg: convergence_dr.dat
	@echo "Plotting dr convergence..."
	@echo 'set terminal svg size 800,600; \
	set output "fig_conv_dr.svg"; \
	set title "Convergence w.r.t dr (rmax=15)"; \
	set xlabel "dr (Bohr)"; set ylabel "Ground State Energy (Hartree)"; \
	set grid; \
	set yrange [-0.6:-0.4]; \
	plot "convergence_dr.dat" using 1:2 with linespoints title "E_0", \
	     -0.5 with lines title "Exact (-0.5)";' | gnuplot

clean:
	rm -f $(OBJECTS) $(EXEC) Out.txt *.dat *.svg tmp.* *.out