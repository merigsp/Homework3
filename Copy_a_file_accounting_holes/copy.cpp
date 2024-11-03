#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

bool is_valid(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

void copy_file(const std::string& from, const std::string& destination) {
    int fd1 = open(from.c_str(), O_RDONLY);
    if (fd1 < 0) {
        std::cout << "Error\n";
        return;
    }

    int fd2 = open(destination.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd2 < 0) {
        std::cout<< "Error\n";
        close(fd1);
        return;
    }

    char buffer[4096];
    ssize_t bytes_read;
    off_t bytes_copied = 0;
    off_t physical_data = 0;
    off_t holes = 0;

    while ((bytes_read = read(fd1, buffer, sizeof(buffer))) > 0) {
        bytes_copied += bytes_read;
        physical_data += bytes_read;

        if (write(fd2, buffer, bytes_read) < 0) {
            std::cout << "Error\n";
            close(fd1);
            close(fd2);
            return;
        }
    }

    if (bytes_read < 0) {
        std::cout << "Error\n" << std::endl;
    }

    holes = lseek(fd1, 0, SEEK_END) - bytes_copied;
    std::cout << "Copied!\n";

    close(fd1);
    close(fd2);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout<< "Error\n";
        return 1;
    }

    std::string file_from = argv[1];
    std::string file_to = argv[2];

    if (!is_valid(file_from)) {
        std::cout << "Error\n";
        return 1;
    }

    copy_file(file_from, file_to);
    return 0;
}

