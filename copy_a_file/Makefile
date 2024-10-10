all: simple_copy

simple_copy: simple_copy.o
	g++  -o simple_copy simple_copy.o

simple_copy.o: simple_copy.cpp
	g++  -c simple_copy.cpp -o simple_copy.o

clean:
	rm -f *.o simple_copy

