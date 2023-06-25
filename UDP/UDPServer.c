#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Festlegung des Ports
#define PORT 5000
// Festlegung der maximalen Zeichen-Anzahl pro Nachricht
#define MAX 80

// Methode zum Erstellen und Laufen lassen des Servers
int main(){
    // Erstellung eines Puffers der Größe 80 (s.o.)
    char buff[MAX];
    // Der Socket-Datei-Verarbeiter (Hauptspeicher Variable des Sockets) & length für später
    int socketFileHandle, length;
    // Zwei Structures für Server und Client Adresse
    struct sockaddr_in serverAddress, clientAddress;
    // Die Structure der Server Adresse wird geleert
    bzero(&serverAddress, sizeof(serverAddress));

    // Erstellen des Sockets, in diesem Fall UDP & Erfolg-/Fehler-Meldung
    socketFileHandle = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFileHandle == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    // Zuteilung von IP-Adresse und Port
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_family = AF_INET;

    // Schreiben der Server Adresse in die Socket Beschreibung
    bind(socketFileHandle, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    // Endlos-Schleife, die nur bei Bedingungen abbricht, damit Chat endlos weitergehen kann
    for(;;) {
        //receive the datagram !!!!!!!!
        //length = sizeof(clientAddress);

        // Leeren des Puffers
        bzero(buff, MAX);

        // Empfangen der Nachricht vom Client, mit allen nötigen Informationen
        recvfrom(socketFileHandle, buff, sizeof(buff), 0, (struct sockaddr *) &clientAddress, sizeof(&clientAddress));
        /* Abfrage ob der Puffer nur aus dem Wort "exit" besteht,
         * mit der Methode "strncmp" welche 2 Strings vergleicht,
         * falls ja, dann wird die Schleife unterbrochen und die chatFunction Methode endet */
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Server Exit...\n");
            break;
        }

        //buff[n] = '\0';

        // Ausgabe des Puffers
        printf("From Client: %s", buff);

        // Informations-Nachricht
        printf("Sending copy of Message as response...\n");
        // Senden der Nachricht an den Client mit einem unveränderten Puffer
        sendto(socketFileHandle, buff, MAX, 0, (struct sockaddr *) &clientAddress, sizeof(clientAddress));
    }
    // Schließen des Sockets
    close(socketFileHandle);
}
