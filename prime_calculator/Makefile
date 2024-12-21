CXX = g++
CXXFLAGS = -std=c++17 
TARGET = prime-calculator

all: $(TARGET)

$(TARGET): main.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

clean:
	rm -f $(TARGET) main.o

