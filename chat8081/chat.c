#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

int sockfd;
struct sockaddr_in my_addr, peer_addr;

void *send_messages(void *arg) {
    char buf[BUF_SIZE];
    while (1) {
        ssize_t len = read(STDIN_FILENO, buf, BUF_SIZE - 1);
        if (len > 0) {
            buf[len] = '\0';  
            sendto(sockfd, buf, len, 0, (struct sockaddr *)&peer_addr, sizeof(peer_addr));
        }
    }
    return NULL;
}

void *recv_messages(void *arg) {
    char buf[BUF_SIZE];
    socklen_t addr_len = sizeof(peer_addr);
    while (1) {
        ssize_t n = recvfrom(sockfd, buf, BUF_SIZE - 1, 0,
                             (struct sockaddr *)&peer_addr, &addr_len);
        if (n > 0) {
            buf[n] = '\0';
            printf("Peer: %s", buf);
            fflush(stdout);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <my_port> <peer_port>\n", argv[0]);
        exit(1);
    }

    int my_port = atoi(argv[1]);
    int peer_port = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(my_port);

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    memset(&peer_addr, 0, sizeof(peer_addr));
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); 
    peer_addr.sin_port = htons(peer_port);

    pthread_t send_thread, recv_thread;
    pthread_create(&send_thread, NULL, send_messages, NULL);
    pthread_create(&recv_thread, NULL, recv_messages, NULL);

    pthread_join(send_thread, NULL);
    pthread_join(recv_thread, NULL);

    close(sockfd);
    return 0;
}

