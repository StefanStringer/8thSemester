CXX = g++ -std=c++23
CXXFLAGS += -O2 -Wall
LDLIBS += -lm -lstdc++ -lpthread

all: Out.txt erf.svg erf_acc.svg

# Compile main.cc to main executable cause i still dont get this
main: main.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

main.o: main.cc matrix.hpp
	$(CXX) $(CXXFLAGS) -c $<

# Run main to generate Out.txt and data files
Out.txt: main
	./$< > $@

# Generate erf.svg from erf.dat
erf.svg: erf.dat
	gnuplot -e "set terminal svg; set output 'erf.svg'; set title 'Error function erf(z)'; set xlabel 'z'; set ylabel 'erf(z)'; plot 'erf.dat' using 1:2 with lines title 'erf(z)'"

# Generate erf_acc.svg from erf_acc.dat
erf_acc.svg: erf_acc.dat
	gnuplot -e "set terminal svg; set output 'erf_acc.svg'; set title 'Accuracy of the error function approximation'; set xlabel 'acc'; set ylabel 'abs(erf(1) - exact)'; set logscale xy; plot 'erf_acc.dat' using 1:3 with linespoints title 'Error vs acc'"

.PHONY: clean test

clean:
	rm -f main main.o Out.txt erf.dat erf.svg erf_acc.dat erf_acc.svg

test: main
	./main
	@echo "Compiler: $(CXX)"
	@echo "Flags: $(CXXFLAGS)"