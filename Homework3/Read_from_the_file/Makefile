all: read_file

read_file: main.o
	g++  -o read_file main.o

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm -f *.o read_file

