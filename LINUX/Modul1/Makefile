all: diff

diff: main.o
	g++  -o diff main.o

main.o: main.cpp
	g++  -c main.cpp -o main.o

clean:
	rm -f *.o diff


