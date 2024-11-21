#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void do_command(char** argv) {
    pid_t pid = fork();
    if (pid == -1) {
        std::cout << "Error";
       return;
    }

    if (pid == 0) {
        execvp(argv[0], argv);
        std::cout << "Error";
        return;
    } else {
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            std::cout << "Error";
            return;
        }

    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
       std::cout << "Error";
       return 1;
    }
    do_command(argv + 1);
    return 0;
}

