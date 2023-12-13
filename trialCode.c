#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Megan: C:/Users/megan/SchoolWork/GIT/final/World

char modifier = 'e';

int x = 0;
int y = 0;

void connectSocket()
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
}

void navigate(char *currentFolder, int deltaY, int deltaX)
{
    x += deltaX;
    y += deltaY;

    char nextFolder[256];
    char parentFolder[256];

    snprintf(parentFolder, sizeof(parentFolder), "C:/Users/megan/SchoolWork/GIT/final/World");

    snprintf(nextFolder, sizeof(nextFolder), "%s/%c%d%d", parentFolder, modifier, x, y);

    DIR *dir = opendir(nextFolder);
    if (dir != NULL)
    {
        closedir(dir);
        strcpy(currentFolder, nextFolder);
    }
    else
    {
        x -= deltaX;
        y -= deltaY;
        printf("Invalid direction. Try again.\n");
    }
}

int readTxt(const char *filename, int directions[4])
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("fopen");
        return -1;
    }
    int i = 0;
    char c;
    while ((c = fgetc(fp)) != EOF)
    {
        directions[i] = c - '0';
        i++;
    }

    fclose(fp);
    return 0;
}

int navigateWorld()
{
    char currentFolder[256];
    strcpy(currentFolder, "C:/Users/megan/SchoolWork/GIT/final/World/e00");

    while (1)
    {
        char txtFileName[256];
        snprintf(txtFileName, sizeof(txtFileName), "%s/neighboring_points.txt", currentFolder);

        int directions[4] = {0};
        if (readTxt(txtFileName, directions) != 0)
        {
            printf("Error reading directions from %s\n", txtFileName);
            break;
        }

        // printf("Directions: Up=%d, Down=%d, Left=%d, Right=%d\n", directions[0], directions[1], directions[2], directions[3]);

        // printf("Enter a direction (w = up, s = down, a = left, d = right, q = quit): ");
        // char input;
        // scanf(" %c", &input);

        // this grabs the input to the user
        read(connfd, message, sizeof(message));

        if (input == 'N' && (directions[0] == 1 || directions[0] == 2 || directions[0] == 3))
        {
            // Move up
            navigate(currentFolder, -1, 0);
        }
        else if (input == 'S' && (directions[1] == 1 || directions[1] == 2 || directions[1] == 3))
        {
            // Move down
            navigate(currentFolder, 1, 0);
        }
        else if (input == 'W' && (directions[2] == 1 || directions[2] == 2 || directions[2] == 3))
        {
            // Move left
            navigate(currentFolder, 0, -1);
        }
        else if (input == 'E' && (directions[3] == 1 || directions[3] == 2 || directions[3] == 3))
        {
            // Move right
            navigate(currentFolder, 0, 1);
        }
        else if (input == 'Q')
        {
            // Quit
            break;
        }
    }

    return 0;
}

int main()
{
    connectSocket();
    navigateWorld();
    return 0;
}
