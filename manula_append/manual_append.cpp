#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main(int argc, char* argv[]){

	if(argc < 2){
		std::cout << "ERROR\n";
		return 1;
	}

	int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd == -1){
 		std::cout << "ERROR\n";
		return 1;
	}

	int fd2 = dup(fd);
	if(fd2 == -1){
                std::cout << "ERROR\n";
                close(fd);
		return 1;
        }

	const char* first_line = "Salagadoola mechicka boola\n";
    	if (write(fd, first_line, strlen(first_line)) == -1) {
        	std::cout << "ERROR\n";
        	close(fd);
        	close(fd2);
        	return 1;
    	}

	const char* second_line = "Bibbidi-Bobbidi-Boo\n";
	if (write(fd2, second_line, strlen(second_line)) == -1) {
		std::cout << "ERROR\n"; 
        	close(fd);
        	close(fd2);
        	return 1;
    	}


	close(fd);	
	close(fd2);

	return 0;
}
