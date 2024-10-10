#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "too many or not enough arguments.\n";
        return 1;
    }

    int file = open(argv[1], O_RDONLY);
    if (file == -1) {
        std::cerr << "Something went wrong while opening the file " << argv[1] << "\n";
        return 1;
    }

    char buffer[256];
    ssize_t bytesRead;

    while ((bytesRead = read(file, buffer, sizeof(buffer))) > 0) {
        std::cout.write(buffer, bytesRead);
    }

    close(file);
    return 0;
}

