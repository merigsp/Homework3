
all: manual_append

manual_append: manual_append.o
	g++ -o manual_append manual_append.o

manual_append.o: manual_append.cpp
	g++ -c manual_append.cpp

clean:
	rm -f manual_append manual_append.o

