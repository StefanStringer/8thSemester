CXX = g++
CXXFLAGS = -O2 -std=c++23 -pthread
TARGET = main
SRC = main.cc

all: benchmark plot

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

#Benchmake target added to have a it more to check
benchmark: $(TARGET)
	@echo ">>> Starting Benchmark..."
	@echo "# threads time_seconds total_sum" > out.times
	@for nthreads in 1 2 3 4 5 6 7 8; do \
		echo "   Running with $$nthreads threads..."; \
		./$(TARGET) -threads $$nthreads -terms 1000000000 >> out.times; \
	done
	@echo ">>> Benchmark complete. Data saved in 'out.times'"

# Optional part was to plot it cause why not
plot: out.times
	@echo ">>> Generating Plot..."
	gnuplot -e " \
		set terminal svg size 1000,700 font 'Arial,14'; \
		set output 'time_plot.svg'; \
		set title 'Harmonic Sum: Execution Time vs Number of Threads'; \
		set xlabel 'Number of Threads'; \
		set ylabel 'Time (seconds)'; \
		set grid; \
		set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1.5; \
		plot 'out.times' using 1:2 with linespoints ls 1 title 'Measured Time'; \
		"
	@echo ">>> Plot saved as 'time_plot.svg'"
	@echo ">>> DONE! Check 'out.times' and 'time_plot.svg'"

clean:
	rm -f $(TARGET) out.times time_plot.svg

.PHONY: all clean benchmark plot