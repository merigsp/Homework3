#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *fifoPath = "calc_pipe";
    int fd = open(fifoPath, O_RDONLY);
    int result;
    read(fd, &result, sizeof(result));
    close(fd);
    
    std::cout << "Result: " << result << std::endl;
    return 0;
}
