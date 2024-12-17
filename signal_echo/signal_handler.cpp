#define _GNU_SOURCE
#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <pwd.h>
#include <signal.h>
#include <cstring>

void signal_handler(int sig, siginfo_t *info, void *context) {
    if (sig == SIGUSR1) {
        pid_t sender_pid = info->si_pid;
        uid_t sender_uid = info->si_uid;
        struct passwd *pw =getpwuid(sender_uid);

        std::cout << "poluchil signal!" << std::endl;
        std::cout << "Sender PID: " << sender_pid << std::endl; //xotela sokretit, vishlo ploxo...
        std::cout << "Sender UID: " << sender_uid << std::endl; 
        if (pw) {
            std::cout << "Sender Username: " << pw->pw_name << std::endl;
        } else {
            std::cout<< "Sender Username: secretik" << std::endl;
        }

        ucontext_t *ucontext = (ucontext_t *)context;
        std::cout << "Context Address: " << ucontext << std::endl;
    }
}

int main() {
    std::cout << "Program PID: "<< getpid() << std::endl;

    struct sigaction action;
    std::memset(&action, 0, sizeof(action));
    action.sa_sigaction = signal_handler;
    action.sa_flags = SA_SIGINFO;

    if (sigaction(SIGUSR1,&action, nullptr) == -1) {
        std::cerr<< "sigaction";
        return 1;
    }

    while (true){
        std::cout << "jdem SIGUSR1..." << std::endl;
        sleep(10); 
	}
    return 0;
}

