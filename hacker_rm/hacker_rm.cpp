#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>

void overwrite(const char *path) {
    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        std::cerr << "smth went wrong while opening the file " << path << std::endl;
        exit(EXIT_FAILURE);
    }

    struct stat fileStat;
    if (stat(path, &fileStat) < 0) {
        std::cerr << "smth went wrong while getting info "<< std::endl;
        close(fd);
        exit(EXIT_FAILURE);
    }

    off_t fileSize = fileStat.st_size;
    char null = '\0';
    for (off_t i = 0; i < fileSize; ++i) {
        if (write(fd, &null, 1) < 0) {
            std::cerr << "smth went wrong while writing " << std::endl;
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    if (close(fd) < 0) {
        std::cerr << "smth went wrong while closing " <<std::endl;
      exit(EXIT_FAILURE);
    }

    if (unlink(path) < 0) {
        std::cerr << "error, i can't delete the file " << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "file '" << path << "' is rewritten and deleted" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Too many or not enough args";
        return 1;
    }

    const char *filePath = argv[1];
    overwrite(filePath);

    return 0;
}

