#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void chatFunction(int sockfd)
{
    char buff[MAX];
    int n = 0;

    for (;;) {

        n = 0;

        bzero(buff, MAX);
        printf("Enter the string : ");

        while ((buff[n++] = getchar()) != '\n');

        write(sockfd, buff, sizeof(buff));
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
        bzero(buff, MAX);

        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s\t", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

void creatingConnection()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    //Get the IP-Adress from user
    char ipAdress[20];
    printf("Please enter an IP-Adress:\n");
    scanf("%s", ipAdress);

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ipAdress);
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // function for chat
    chatFunction(sockfd);

    // close the socket
    close(sockfd);
}

int main()
{
    int option;

    while (1) {
        printf("Select an option:\n");
        printf("[1] Start the TCP-Client\n");
        printf("[2] Start PCAP-LOG\n");
        printf("[3] Get Service-Information\n");
        printf("[4] Get the Host-Name\n");
        printf("[0] To EXIT\n");

        if (scanf("%d", &option) != 1) {
            printf("Invalid input. Please enter a number.\n");
            // Clear the input buffer
            while (getchar() != '\n');
            continue;
        }
        if(option == 0){
            exit(EXIT_SUCCESS);
        }

        switch (option) {
            case 1:
                creatingConnection();
                break;
            case 2:
                system("gnome-terminal -- sudo ./pcapForTCP");
                break;
            case 3:
                //getservbyname();
                break;
            case 4:
                //getHostName();
                break;
            default:
                printf("Invalid option. Please select a valid option (1-3).\n");
                break;
        }
    }
}
