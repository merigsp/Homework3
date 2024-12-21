#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <cstring>

int findPrimeN(int n) {
    if (n <= 0) return -1;
    int count = 0, tiv = 1;
    while (count < n) {
        ++tiv;
        bool prime = true;
        for (int i = 2; i * i <= tiv; ++i) {
            if (tiv % i == 0) {
                prime = false;
                break;
            }
        }
        if (prime) ++count;
    }
    return tiv;
}

int main() {
    int pipeParToChi[2], pipeChiToPar[2];

    if (pipe(pipeParToChi) == -1 || pipe(pipeChiToPar) == -1) {
        std::cerr << "[Error] Pipe creation failed." << std::endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "[Error] Fork failed." << std::endl;
        return 1;
    }

    if (pid == 0) { 
        close(pipeParToChi[1]); 
        close(pipeChiToPar[0]);

        while (true) {
            int m;
            if (read(pipeParToChi[0], &m, sizeof(m)) <= 0) {
                break;
            }

            std::cout << "[Child] Calculating " << m << "-th prime number..." << std::endl;
            int result = findPrimeN(m);

            if (write(pipeChiToPar[1], &result, sizeof(result)) == -1) {
                std::cerr << "[Child] Error sending result." << std::endl;
                break;
            }
            std::cout << "[Child] Sending calculation result of prime(" << m << ")..." << std::endl;
        }

        close(pipeParToChi[0]);
        close(pipeChiToPar[1]);
    } else { 
        close(pipeParToChi[0]);  
        close(pipeChiToPar[1]); 

        while (true) {
            std::cout << "[Parent] Please enter the number: ";
            std::string input;
            std::getline(std::cin, input);

            if (input == "exit") {
                break;
            }

            int m;
            try {
                m = std::stoi(input);
            } catch (...) {
                std::cerr << "[Parent] Invalid input. Please enter an integer." << std::endl;
                continue;
            }

            std::cout << "[Parent] Sending " << m << " to the child process..." << std::endl;
            if (write(pipeParToChi[1], &m, sizeof(m)) == -1) {
                std::cerr << "[Parent] Error sending data to child." << std::endl;
                break;
            }

            std::cout << "[Parent] Waiting for the response from the child process..." << std::endl;
            int result;
            if (read(pipeChiToPar[0], &result, sizeof(result)) <= 0) {
                std::cerr << "[Parent] Error receiving data from child." << std::endl;
                break;
            }

            std::cout << "[Parent] Received calculation result of prime(" << m << ") = " << result << "..." << std::endl;
        }

        close(pipeParToChi[1]);
        close(pipeChiToPar[0]);
    }

    return 0;
}

