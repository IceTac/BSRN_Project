#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>

#define PORT 5000
#define MAX 80

void runningChatFunction()
{
    char buffer[100];
    int sockfd, n;
    struct sockaddr_in servaddr;

    // clear servaddr
    bzero(&servaddr, sizeof(servaddr));

    //Get the IP-Adress from user
    char ipAdress[20];
    printf("Please enter an IP-Adress:\n");
    scanf("%s", ipAdress);
    while (getchar() != '\n');

    //Assigning Port and IP-Adress
    servaddr.sin_addr.s_addr = inet_addr(ipAdress);
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;

    // create datagram socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    printf("Creating Socket...\n");

    // connect to server
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        exit(0);
    }else{
        printf("Connection with Server successful\n");
    }

    for(;;) {
        int x = 0;

        // request to send datagram
        // no need to specify server address in sendto
        // connect stores the peers IP and port
        printf("To Server: ");
        while ((buffer[x++] = getchar()) != '\n');
        sendto(sockfd, buffer, MAX, 0, (struct sockaddr *) NULL, sizeof(servaddr));
        if ((strncmp(buffer, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }

        // waiting for response
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) NULL, NULL);
        printf("From Server: %s\n", buffer);
        if ((strncmp(buffer, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
    // close the descriptor
    close(sockfd);
}

// Methode um Name, Port und Protokoll eines Services herauszufinden
void serviceInformation(){
    // Variable für den Namen der vom Nutzer eingegeben wird
    char serviceName[20];
    printf("Please enter a Service:\n");
    // Standard Eingabe
    scanf("%s", serviceName);
    // Structure um die getservbyname Methode auszuführen und die Rückgabewerte zu speichern
    struct servent *service = getservbyname(serviceName, "udp");
    // Abfrage um zu gucken, ob es Infos zum eingegeben Service gibt oder nicht, Ausgabe & Fehler-Meldung
    if (service){
        printf("Service name is %s. Service port is %d. Protocol to use is %s\n",
               service->s_name, ntohs(service->s_port), service->s_proto);
    }else{
        printf("No entry found for this service.\n");
    }
}
// Methode um Host-Name und IP eines Namens herauszufinden
void hostName(){
    // Variable für den Namen der vom Nutzer eingegeben wird
    char name[20];
    printf("Please enter a Name:\n");
    // Standard Eingabe
    scanf("%s", name);
    // Structure um die gethostbyname Methode auszuführen und die Rückgabewerte zu speichern
    struct hostent *host = gethostbyname(name);
    // Abfrage um zu gucken, ob es Infos zum eingegeben Host gibt oder nicht, Ausgabe & Fehler-Meldung
    if (host){
        printf("Official-Host-Name is %s. ", host->h_name);
        // Schleife um alle IP-Adressen die der Host besitzt anzuzeigen
        for (int i = 0; host->h_addr_list[i] != NULL; i++) {
            // inet_ntoa übersetzt die IPs in normale Strings
            printf("IP-Address %d: %s\n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
        }
    }else{
        printf("No entry found for this Host.\n");
    }
}

// Hauptteil des Programms, Verarbeitung und Steuerung der jeweiligen Eingabe
int main() {
    // Variable um den Input zu speichern
    int option;

    while (1) {
        // Prints für die verschiedenen verfügbaren Auswahlmöglichkeiten
        printf("Select an option:\n");
        printf("[1] Start the UDP-Client\n");
        printf("[2] Start PCAP-LOG\n");
        printf("[3] Get Service-Information\n");
        printf("[4] Get the Host-Name\n");
        printf("[0] To EXIT\n");

        /* If-Abfrage und Eingabe miteinander, um sicherzustellen, dass eine Zahl eingegeben wurde,
         * gleichzeitig speichert "option" die Eingabe des Nutzers
         * und falls es keine Zahl war, springt die Schleife zurück and den Anfang */
        if (scanf("%d", &option) != 1) {
            printf("Invalid input. Please enter a number.\n");
            // Leeren des Input-Puffers, um für neue Eingaben bereit zu sein
            while (getchar() != '\n');
            continue;
        }

        // Switch-Case Funktion um die getätigte Auswahl schließlich durchzuführen
        switch (option) {
            case 0:
                // Standard Exit-Methode mit Standard Exit-Code 0
                exit(EXIT_SUCCESS);
            case 1:
                // Klassischer Methodenaufruf
                runningChatFunction();
                break;
            case 2:
                // Aufruf eines neuen Terminals für das PCAP-Programm
                system("gnome-terminal -- sudo ./pcapForUDP");
                break;
            case 3:
                // Klassischer Methodenaufruf
                serviceInformation();
                break;
            case 4:
                // Klassischer Methodenaufruf
                hostName();
                break;
            default:
                printf("Invalid option. Please select a valid option (1-4).\n");
                break;
        }
    }
}
