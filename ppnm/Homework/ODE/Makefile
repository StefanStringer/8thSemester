CXX = g++
CXXFLAGS = -std=c++23 -O2 -Wall

OUT = main

DATA_A = Question_A.dat
DATA_B = Question_B.dat
DATA_C = Question_C.dat

FIG_A = Question_A.svg
FIG_B = Question_B.svg
FIG_C = Question_C.svg

all: $(OUT) run plots

$(OUT): main.cpp matrix.hpp
	$(CXX) $(CXXFLAGS) -o $@ main.cpp

run: $(OUT)
	./$(OUT)

# Question A - Pendulum
$(FIG_A): $(DATA_A)
	gnuplot -e "set terminal svg size 800,600; \
	set output '$(FIG_A)'; \
	set title 'Damped Pendulum'; \
	set xlabel 't'; set ylabel 'y'; set grid; \
	plot '$(DATA_A)' using 1:2 with lines lw 2 lc rgb 'blue' title 'theta(t)', \
	     '$(DATA_A)' using 1:3 with lines lw 2 lc rgb 'green' title 'omega(t)'"

# Question B - Orbits / Ellipses
$(FIG_B): $(DATA_B)
	gnuplot -e "set terminal svg size 800,600; \
	set output '$(FIG_B)'; \
	set title 'Orbit Solutions'; \
	set xlabel 'x'; set ylabel 'y'; set grid; \
	plot '$(DATA_B)' index 0 using (1/\$$2)*cos(\$$1):(1/\$$2)*sin(\$$1) with lines lw 2 lc rgb 'red' title 'Ellipse 1', \
	     '$(DATA_B)' index 1 using (1/\$$2)*cos(\$$1):(1/\$$2)*sin(\$$1) with lines lw 2 lc rgb 'orange' title 'Ellipse 2', \
	     '$(DATA_B)' index 2 using (1/\$$2)*cos(\$$1):(1/\$$2)*sin(\$$1) with lines lw 2 lc rgb 'blue' title 'Precessing Orbit'"

# Question C - Three-body figure-8
$(FIG_C): $(DATA_C)
	gnuplot -e "set terminal svg size 800,600; \
	set output '$(FIG_C)'; \
	set title 'Three-body Figure-8'; \
	set xlabel 'x'; set ylabel 'y'; set grid; \
	plot '$(DATA_C)' using 1:2 with lines lw 2 lc rgb 'red' title 'Body 1', \
	     '$(DATA_C)' using 3:4 with lines lw 2 lc rgb 'green' title 'Body 2', \
	     '$(DATA_C)' using 5:6 with lines lw 2 lc rgb 'blue' title 'Body 3'"

plots: $(FIG_A) $(FIG_B) $(FIG_C)

.PHONY: clean
clean:
	rm -f $(OUT) $(DATA_A) $(DATA_B) $(DATA_C) $(FIG_A) $(FIG_B) $(FIG_C)