#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int evalPolNot(const std::string &expr) {
    std::istringstream iss(expr);
    std::stack<int> s;
    std::string tok;
    while (iss >> tok) {
        if (isdigit(tok[0])) {
            s.push(tok[0] - '0');
        } else {
            int b = s.top(); s.pop();
            int a = s.top(); s.pop();
            switch (tok[0]) {
                case '+': s.push(a + b); break;
                case '-': s.push(a -b); break;
                case '*': s.push(a * b); break;
                case '/': s.push(a/ b); break;
            }
        }
    }
    return s.top();
}

int main() {
    const char *fifoPath = "calc_pipe";
    mkfifo(fifoPath, 0666);
    
    std::string expr;
    std::cout <<  "Enter expression in Polish notation: ";
    getline(std::cin , expr);
    int result = evalPolNot(expr);
    
    int fd = open(fifoPath, O_WRONLY);
    write(fd, &result, sizeof(result));
    close(fd );
    return 0;
}
