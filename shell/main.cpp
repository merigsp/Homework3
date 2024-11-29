#include <iostream>
#include <string>
#include <cstdlib> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <fcntl.h> 
#include <unistd.h> 

void do_command(const std::string& command, bool silent) {
    pid_t pid = fork(); 

    if (pid == -1) { 
        std::cerr << "Error";
        return;
    }

    if (pid == 0) { 
        std::string current_dir = getcwd(nullptr, 0);
        std::string path = "PATH =" + current_dir + ":" + getenv("PATH");
        putenv(const_cast<char*>(path.c_str()));

        if (silent) {
            std::string file = std::to_string(getpid()) + ".log";
            int fd = open(file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
            std::cerr << "Error";           
            return;
            }
            if (dup2(fd, STDOUT_FILENO) == -1 || dup2(fd, STDERR_FILENO) == -1) {
                std::cerr << "Error";
                close(fd);
                return;
            }
            close(fd);
        }

        execlp("/bin/sh", "sh", "-c", command.c_str(), nullptr);
        std::cerr << "Error\n";
        return;
    } else {
        waitpid(pid, nullptr, 0); 
    }
}

int main() {
    while (true) {
        std::cout << "enter command> "; 
        std::string input;
        std::getline(std::cin, input);

        if (input == "exit") { 
            std::cout << "...\n";
            break;
        }

        if (input.empty()) continue; 

        bool silent = false;
        if (input.find("silent ") == 0) {
            silent = true;
            input = input.substr(7);
        }

        do_command(input, silent); 
    }

    return 0;
}

