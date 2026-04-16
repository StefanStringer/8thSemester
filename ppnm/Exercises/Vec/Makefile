CXX = g++ -std=c++23
CXXFLAGS += -Wall -Wextra -Werror
LDLIBS += -lstdc++ -lm 

run : Out.txt
	cat Out.txt

Out.txt: main
	./$< > $@

main: main.o vec.o

main.o : main.cpp vec.h

vec.o : vec.cpp vec.h

.PHONY: clean
clean:
	$(RM) -f main main.o vec.o Out.txt

test:
	@echo $(CXX)
	@echo $(CXXFLAGS)
	@echo $(RM)

# # ------------------------------------------------------------
# # Makefile for the vec demo
# # ------------------------------------------------------------

# # ---- Compiler configuration ------------------------------------
# CXX      := g++
# CXXFLAGS := -std=c++23 -Wall -Wextra -Werror -O2   # change -O2 → -O0 for debugging
# LDFLAGS  := -lstdc++ -lm                         # libraries to link

# # ---- Names ----------------------------------------------------
# TARGET   := vec_demo          # final executable
# OUTFILE  := Out.txt           # where we store the program output

# # ---- Source / object lists ------------------------------------
# SRC   := main.cpp vec.cpp
# OBJ   := $(SRC:.cpp=.o)      # expands to main.o vec.o

# # ------------------------------------------------------------
# # Default target – build the executable
# # ------------------------------------------------------------
# all: $(TARGET)

# # ------------------------------------------------------------
# # Linking step – creates the executable from the objects
# # ------------------------------------------------------------
# $(TARGET): $(OBJ)
# 	$(CXX) $(LDFLAGS) -o $@ $^

# # ------------------------------------------------------------
# # Compilation step – builds each .o from its .cpp
# # ------------------------------------------------------------
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c -o $@ $<

# # ------------------------------------------------------------
# # Convenience targets
# # ------------------------------------------------------------
# .PHONY: clean run save watch

# # Run the program, output goes straight to the terminal
# run: $(TARGET)
# 	./$(TARGET)

# # Run the program and **save everything** (stdout + stderr) into Out.txt
# save: $(TARGET)
# 	./$(TARGET) &> $(OUTFILE)   # &> redirects both streams

# # Run the program, show output on screen **and** write it to Out.txt
# watch: $(TARGET)
# 	./$(TARGET) | tee $(OUTFILE)

# # Remove generated files
# clean:
# 	rm -f $(OBJ) $(TARGET) $(OUTFILE)

# test:
# 	@echo $(CXX)
# 	@echo $(CXXFLAGS)
# 	@echo $(RM)