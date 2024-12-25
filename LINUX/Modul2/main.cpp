#include <iostream>
#include <thread>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

const int BUFFER_SIZE = 1024;
char buffer[BUFFER_SIZE];
bool finishedReading = false;
int bytesRead = 0;

void readFromFile() {
	int fd = open("input.txt", O_RDONLY);
    	if (fd == -1) {
        	perror("Error, ne mogu opening file");
        	return;
    	}

    	while (true) {
        	int n = read(fd, buffer, BUFFER_SIZE);
       		if (n <= 0) { 
            	finishedReading = true;
            	close(fd);
            	return;
        }
        bytesRead = n;
    }
}

void printBuffer() {
	while (!finishedReading || bytesRead > 0) {
        	if (bytesRead > 0) {
            	std::cout.write(buffer, bytesRead);
            	std::cout << "quatity of read symbols: " << bytesRead << std::endl;
            	bytesRead = 0;
		
        }
    }
}

int main() {
    	std::thread reader(readFromFile);
	std::thread writer(printBuffer);
	
	reader.join();
	writer.join();
	std::cout<< "Vsyo?" << std::endl;

    return 0;
}


