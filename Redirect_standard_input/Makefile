all:redirect_cin

redirect_cin: redirect_cin.o
	g++ -o redirect_cin redirect_cin.o

redirect_cin.o: redirect_cin.cpp
	g++ -c redirect_cin.cpp -o redirect_cin.o

clean:
	rm -f *.o redirect_cin
