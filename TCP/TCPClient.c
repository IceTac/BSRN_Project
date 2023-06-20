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
    int n;

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
    int sockfd;
    struct sockaddr_in servaddr;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else {
        printf("Socket successfully created..\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    //Get the IP-Adress from user
    char ipAdress[20];
    printf("Please enter an IP-Adress:\n");
    scanf("%s", ipAdress);
    while (getchar() != '\n');

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ipAdress);
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else {
        printf("connected to the server..\n");
    }

    // function for chat
    chatFunction(sockfd);

    // close the socket
    close(sockfd);
}

void serviceInformation(){
    char serviceName[20];
    printf("Please enter a Service:\n");
    scanf("%s", serviceName);
    struct servent *service = getservbyname(serviceName, "tcp");
    if (service){
        printf("Service name is %s. Service port is %d. Protocol to use is %s\n",
               service->s_name, ntohs(service->s_port), service->s_proto);
    }else{
        printf("No entry found for this service.\n");
    }
}

void hostName(){
    char name[20];
    printf("Please enter a Name:\n");
    scanf("%s", name);
    struct hostent *host = gethostbyname(name);
    if (host){
        printf("Official-Host-Name is %s. ", host->h_name);
        for (int i = 0; host->h_addr_list[i] != NULL; i++) {
            printf("IP-Address %d: %s\n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
        }
    }else{
        printf("No entry found for this Host.\n");
    }
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
                serviceInformation();
                break;
            case 4:
                hostName();
                break;
            default:
                printf("Invalid option. Please select a valid option (1-3).\n");
                break;
        }
    }
}
