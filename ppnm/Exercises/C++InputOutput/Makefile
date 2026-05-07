CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -Werror

# Default target
all:
	@echo "Usage: make part1 | make part2 | make part3"

# --- PART 1 ---
part1: CXXFLAGS += -DPART1
part1: clean main
	./main -n 1 -n 2 -n 3 -n 4 -n 5 > Out.txt
	@echo "=== PART 1 OUTPUT ==="
	@cat Out.txt

# --- PART 2 ---
part2: CXXFLAGS += -DPART2
part2: clean main
	echo 1 2 3 4 5 | ./main > Out.txt
	@echo "=== PART 2 OUTPUT ==="
	@cat Out.txt

# --- PART 3 ---
part3: CXXFLAGS += -DPART3
part3: clean main
	# Ensure infile.txt exists (using your provided file)
	@if [ ! -f infile.txt ]; then echo "1 2 3 4 5" > infile.txt; fi
	./main --input infile.txt --output outfile.txt
	cp outfile.txt Out.txt
	@echo "=== PART 3 OUTPUT ==="
	@cat Out.txt

# --- Build Rules ---
# Ensure this matches your filename (main.cc)
main: main.o
	$(CXX) $(CXXFLAGS) -o main main.o

main.o: main.cc
	$(CXX) $(CXXFLAGS) -c main.cc

# --- Clean ---
clean:
	rm -f main *.o Out.txt outfile.txt

.PHONY: all clean part1 part2 part3