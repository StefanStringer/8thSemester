CXX = g++
CXXFLAGS = -Wall -O2 -std=c++23

TARGET = main
OUT = Out.txt

all: $(TARGET) plot

$(TARGET): main.cc qr.cc matrix.hpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cc qr.cc

plot: $(TARGET)
	./$(TARGET) > $(OUT)
	gnuplot -e "set terminal svg size 800, 600; set output 'sine_declipping.svg'; \
	set title 'Sine Wave Declipping'; set xlabel 'Sample Index'; set ylabel 'Amplitude'; \
	set grid; plot 'sine_declipping.dat' using 1:2 with lines title 'Original Signal', \
	'sine_declipping.dat' using 1:3 with lines title 'Clipped Signal', \
	'sine_declipping.dat' using 1:4 with lines title 'Declipped Signal'"
	gnuplot -e "set terminal svg size 800, 600; set output 'complex_declipping.svg'; \
	set title 'Complex Signal Declipping'; set xlabel 'Sample Index'; set ylabel 'Amplitude'; \
	set grid; plot 'complex_declipping.dat' using 1:2 with lines title 'Original Signal', \
	'complex_declipping.dat' using 1:3 with lines title 'Clipped Signal', \
	'complex_declipping.dat' using 1:4 with lines title 'Declipped Signal'"

clean:
	rm -f $(TARGET) $(OUT) sine_declipping.dat sine_declipping.svg

