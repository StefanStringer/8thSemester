CXXFLAGS = -Wall -Wextra -Werror -std=c++23 -O
LDLIBS = -lstdc++ -lm
Out.txt : main
	./main > Out.txt

main : main.o
	$(CXX) main.o -o main $(LDLIBS)

main.o : main.cc
	$(CXX) $(CXXFLAGS) -c main.cc -o main.o

.PHONY : clean
clean:
	$(RM) --force main main.o Out.txt

test:
	@echo CXX
	@echo $(CXX)
	@echo $(RM)