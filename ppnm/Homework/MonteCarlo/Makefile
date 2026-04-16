CXX = g++
CXXFLAGS = -O2 -std=c++17

TARGET = main
OUT = Out.txt
DATA = data.txt

all: $(TARGET)
	./$(TARGET) > $(OUT)
	gnuplot -e "\
	set terminal svg size 800,600; \
	set output 'error.svg'; \
	set logscale x; \
	set logscale y; \
	set xlabel 'N'; \
	set ylabel 'Error'; \
	set title 'Monte Carlo Error Scaling'; \
	plot '$(DATA)' using 1:2 with lines title 'Estimated error', \
	     '$(DATA)' using 1:3 with lines title 'Actual error', \
	     1/sqrt(x) title '1/sqrt(N)' \
	"

$(TARGET): main.cc
	$(CXX) $(CXXFLAGS) main.cc -o $(TARGET)

clean:
	rm -f $(TARGET) $(OUT) $(DATA) *.svg