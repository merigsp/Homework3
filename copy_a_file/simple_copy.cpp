#include <iostream>
#include <fcntl.h> 
#include <unistd.h>


int main(int argc, char* argv[]){

    if(argc !=3){
        std::cerr << "too many or not enough arguments.";
        
        return 1;
         
        
        
    }
        
    const char* source = argv[1];
    int srcFile = open(argv[1], O_RDONLY);
        
    if(srcFile == -1){
        
        std::cout<<"Something went wrong while opening the source file."<< argv[1] <<"\n";
        
        return 1;
        
        
    }
        
    const char* destination = argv[2];
    int destFile = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        
    if(destFile == -1){
        std::cerr << "Something went wrong while opening the destination file." << argv[2] << "\n";
        close(srcFile);
        
        return 1;
    
        
    }
    char buffer[1024];
    ssize_t bytesRead;

    while ((bytesRead = read(srcFile, buffer, sizeof(buffer))) > 0) {
        if (write(destFile, buffer, bytesRead) != bytesRead) {
            std::cerr << "Something went wrong with write operator.\n";
            close(srcFile);
            close(destFile);
            return 1;
        }
    }

    if (bytesRead < 0) {
        std::cerr << "Something went wrong with read operator.\n";
    }

    close(srcFile);
    close(destFile);

    std::cout << "File copied from " << argv[1] << " to " << argv[2] << "\n";
    return 0;
         
}        

