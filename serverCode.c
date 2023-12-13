#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8888
#define MAX_LEN 1024

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    // Creating TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Binding socket with address and port
    if (bind(sockfd, (struct sockaddr) & servaddr, sizeof(servaddr)) != 0)
    {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // Listening for incoming connections
    if (listen(sockfd, 5) != 0)
    {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for client to connect...\n");
    socklen_t len = sizeof(cliaddr);
    connfd = accept(sockfd, (struct sockaddr) & cliaddr, &len);

    if (connfd < 0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");
    char message[MAX_LEN];

    while (1)
    {
        // Receive message from peer 1
        read(connfd, message, sizeof(message));
        printf("Peer 1: %s", message);
        printf("You: ");
        fgets(message, MAX_LEN, stdin);
        // Sending message to peer 1
        write(connfd, message, strlen(message));
    }

    close(sockfd);
    return 0;
}