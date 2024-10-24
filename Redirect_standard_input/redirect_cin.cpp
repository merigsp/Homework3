#include <iostream>
#include <fstream>
#include <string>
#include <fcntl.h>
#include <unistd.h>

void initialize(int argc, char* argv[]){

	if(argc != 2){
		std::cout << "ERROR";
	        exit(1);
	}

	int file = open(argv[1], O_RDONLY);

	if(file == -1){
		std::cout << "ERROR";
		exit(1);
	}
	
	if((dup2(file, STDIN_FILENO) == -1)){
		std::cout << "ERROR";
		close(file);
		exit(1);

	}

    close(file);
}


int main(int argc, char* argv[]){


	initialize(argc, argv);

	std::string input;
	std::getline(std::cin, input);

        for (int i = input.size() - 1; i >= 0; --i) {
        	std::cout << input[i];
    	}

    	std::cout << std::endl;

        return 0;
}
