CXX= g++ -std=c++23
CXXFLAGS+= -O2 -Wall
LDLIBS= -lm

TARGET = main
OUT_FILE = Out.txt
DATA_FILE = higgs.data.txt
PLOT_DATA = higgs_fit.dat
SVG_FILE = higgs_fit.svg

# Change 'all' to depend on 'run' instead of just the executable
all: run

# Compile the program
$(TARGET): main.cc matrix.hpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cc $(LDLIBS)

# Run the program with data and generate the plot
run: $(TARGET)
	@echo "Running Higgs Fit..."
	./$(TARGET) < $(DATA_FILE) 1> $(OUT_FILE) 2> log
	@echo "Output saved to $(OUT_FILE)"
	@echo "Generating plot..."
	gnuplot -e "set terminal svg size 800,600 enhanced font 'Arial,12'; set output '$(SVG_FILE)'; set title 'Higgs Boson Fit'; set xlabel 'Energy [GeV]'; set ylabel 'Cross Section'; set grid; plot '$(PLOT_DATA)' using 1:2:3 with yerrorbars title 'Data' lt 1, '' using 1:4 with lines title 'Breit-Wigner Fit' lt 2 lw 2"
	@echo "Plot saved as $(SVG_FILE)"

# Clean up
clean:
	rm -f $(TARGET) $(OUT_FILE) $(PLOT_DATA) $(SVG_FILE) log