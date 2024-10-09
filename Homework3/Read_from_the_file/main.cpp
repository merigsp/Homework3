#include <iostream>
#include <fstream>
#include <cstring>

int main(int argc, char* argv[]){
        if(argc < 2){
           
        	std::cout<<"File name was not given by arguments\n";
        
		return 1;
    	}
	std::ifstream file(argv[1]);
    		if (!file.is_open()) {
        	std::cerr << "Error, something went wrong while opening the file " << argv[1] << "\n";
        	return 1;
    	}

    	std::string line;
    		while (std::getline(file, line)) {
        	std::cout << line << "\n";
    	}	

    file.close();
    return 0;
}
