CXX=g++
CXXFLAGS=-O2 -std=c++20 -Wall

SOURCES=main.cc qr.cc
OBJECTS=$(SOURCES:.cc=.o)
EXEC=main

all: Out.txt decay_fit.svg

# compile objects
%.o: %.cc matrix.hpp qr.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# link executable
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

# run program
Out.txt: $(EXEC)
	./$(EXEC) > $@

# plot decay fit
decay_fit.svg: fit_data.txt fit_curve.txt
	echo 'set terminal svg size 800,600; \
	set output "$@"; \
	set xlabel "Time (days)"; \
	set ylabel "Activity"; \
	set grid; \
	plot \
	"$<" using 1:2:3 with yerrorbars title "Experimental data", \
	"fit_curve.txt" using 1:2 with lines lw 2 title "Fit";' | gnuplot

clean:
	rm -f *.o $(EXEC) Out.txt fit_data.txt fit_curve.txt decay_fit.svg