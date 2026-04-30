CXX= g++ -std=c++23
CXXFLAGS+= -O2 -Wall
LDLIBS= -lm

TARGET = main
OUT_FILE = Out.txt

all: $(TARGET) $(OUT_FILE)

$(TARGET): main.cc matrix.hpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cc $(LDLIBS)

$(OUT_FILE): $(TARGET)
	./$(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET) $(OUT_FILE)
