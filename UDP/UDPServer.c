#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/socket.h>
#include<netinet/in.h>

#define PORT 5000
#define MAX 80

// Driver code
int main()
{
    char buffer[100];
    int listenfd, len;
    struct sockaddr_in servaddr, cliaddr;
    bzero(&servaddr, sizeof(servaddr));

    // Create a UDP Socket
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    printf("Creating Socket...\n");

    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;

    // bind server address to socket descriptor
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));


    for(;;) {
        //receive the datagram
        len = sizeof(cliaddr);

        bzero(buffer, MAX);

        //receive message from server
        int n = recvfrom(listenfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &cliaddr, &len);
        if ((strncmp(buffer, "exit", 4)) == 0) {
            printf("Server Exit...\n");
            break;
        }
        buffer[n] = '\0';
        printf("From Client: %s", buffer);

        // send the response
        printf("Sending copy of Message as response...\n");
        sendto(listenfd, buffer, MAX, 0, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
    }
}
