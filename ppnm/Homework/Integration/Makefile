CXX = g++
CXXFLAGS = -std=c++23 -O2 -Wall

OUT = integration_main

all: $(OUT)
	@echo "Running integration program and saving to Out.txt..."
	./$(OUT) > Out.txt
	@echo "Generating plots..."
	python3 plot_erf.py
	@echo "Done. Results in Out.txt and plots saved as .svg files."

$(OUT): main.cc
	$(CXX) $(CXXFLAGS) -o $@ main.cc

clean:
	rm -f $(OUT) Out.txt erf.svg.dat erf_acc.svg.dat *.svg

.PHONY: all clean