CXX = g++
CXXFLAGS = -O2 -std=c++17

TARGET = main
OUT = Out.txt
# Set to data_A.txt to plot Question A, or data_B.txt for Question B
DATA = data_B.txt

all: $(TARGET)
	./$(TARGET) > $(OUT)
	gnuplot -e "\
	set terminal png size 800,600; \
	set output 'error_QB.png'; \
	set logscale x; \
	set logscale y; \
	set xlabel 'N (samples)'; \
	set ylabel 'Actual Error'; \
	set title 'Question B: Quasi-Random vs Pseudo-Random'; \
	set key top right; \
	plot '$(DATA)' using 1:2 with linespoints title 'PRNG Error (1/√N)', \
	     '$(DATA)' using 1:3 with linespoints title 'QR Error (~1/N)' \
	"

$(TARGET): main.cc
	$(CXX) $(CXXFLAGS) main.cc -o $(TARGET)

clean:
	rm -f $(TARGET) $(OUT) $(DATA) *.png *.svg