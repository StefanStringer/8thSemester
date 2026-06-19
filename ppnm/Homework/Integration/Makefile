CXX = g++ -std=c++23
CXXFLAGS += -O2 -Wall

all: Out.txt erf.svg erf_acc.svg

# Compile
main: main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cc
	$(CXX) $(CXXFLAGS) -c $<

# Run program to generate data and text output
Out.txt: main
	./$< > $@

# Ensure data files exist
erf.dat erf_acc.dat: main
	./main >/dev/null 2>&1 || true

# Plotting - Fixed gnuplot syntax
erf.svg: erf.dat
	gnuplot -e "set terminal svg size 800,600; set output 'erf.svg'; set title 'Error function erf(z)'; set xlabel 'z'; set ylabel 'erf(z)'; plot 'erf.dat' using 1:2 with lines title 'My Implementation', 0.8427007929 with lines lt rgb 'red' notitle"

erf_acc.svg: erf_acc.dat
	gnuplot -e "set terminal svg size 800,600; set output 'erf_acc.svg'; set title 'Accuracy vs Tolerance'; set xlabel 'acc'; set ylabel 'Absolute Error'; set log xy; plot 'erf_acc.dat' using 1:3 with linespoints title 'Error vs acc'"

.PHONY: clean all

clean:
	rm -f main main.o Out.txt erf.dat erf_acc.dat erf.svg erf_acc.svg