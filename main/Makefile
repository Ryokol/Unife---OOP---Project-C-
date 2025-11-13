CXX = g++
CXXFLAGS = -std=c++17 -I include -Wall -Wextra -Wpedantic -g
SRCS = main.cpp $(wildcard src/*.cpp)
TARGET = main_app

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: all run clean
