#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <cstring>
#include <pthread.h>

struct message {
    long msg_type;
    char text[10];
};

void* ping_signaler(void* arg) {
    int msgid = *((int*)arg);
    struct message msg;
    while (true) {
        sleep(1);
        std::strcpy(msg.text, "PING");
        msg.msg_type = 1;
        if (msgsnd(msgid, &msg, sizeof(msg.text), 0) == -1) {
            perror("msgsnd");
            pthread_exit(NULL);
        }
        std::cout << "PING" << std::endl;
        if (msgrcv(msgid, &msg, sizeof(msg.text), 2, 0) == -1) {
            perror("msgrcv");
            pthread_exit(NULL);
        }
    }
    return NULL;
}

void* pong_signaler(void* arg) {
    int msgid = *((int*)arg);
    struct message msg;
    while (true) {
        if (msgrcv(msgid, &msg, sizeof(msg.text), 1, 0) == -1) {
            perror("msgrcv");
            pthread_exit(NULL);
        }
        std::cout << "PONG" << std::endl;
        sleep(1);
        std::strcpy(msg.text, "PONG");
        msg.msg_type = 2;
        if (msgsnd(msgid, &msg, sizeof(msg.text), 0) == -1) {
            perror("msgsnd");
            pthread_exit(NULL);
        }
    }
    return NULL;
}

int main() {
    key_t key = ftok("pingpong", 65);
    if (key == -1) {
        perror("ftok");
        return 1;
    }
    
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        return 1;
    }
    
    pthread_t ping, pong;
    if (pthread_create(&ping, NULL, ping_signaler, &msgid) != 0) {
        perror("pthread_create");
        return 1;
    }
    usleep(500000);
    if (pthread_create(&pong, NULL, pong_signaler, &msgid) != 0) {
        perror("pthread_create");
        return 1;
    }
    
    pthread_join(ping, NULL);
    pthread_join(pong, NULL);
    
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
