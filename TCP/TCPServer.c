#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

// Festlegung der maximalen Zeichen-Anzahl pro Nachricht
#define MAX 80
// Festlegung des Ports
#define PORT 8080
// Festlegung des Kürzels SA (Socket Adress) für die Structure sockaddr [Nur für Convenience]
#define SA struct sockaddr

// Methode welche den File Handle von der Connection als Parameter für read() und write() benutzt
void chatFunction(int connectionFileHandle){
    // Erstellung eines Puffers der Größe 80 (s.o.)
    char buff[MAX];

    // Zähler Variable tmp, später nur wichtig für die Eingabe in den Puffer
    int tmp;
    // Endlos-Schleife, die nur bei Bedingungen abbricht, damit Chat endlos weitergehen kann
    for (;;) {
        // Pro Durchang wird tmp auf 0 gesetzt
        tmp = 0;
        // Methode zum Entleeren des Puffers (für fehlerfreie, dopplungsfreie Übertragung wichtig)
        bzero(buff, MAX);

        // Methode zum Lesen vom Client und Speichern im Puffer
        read(connectionFileHandle, buff, sizeof(buff));

        // Ausgabe der Nachricht vom Client (Puffer print)
        printf("From client: %s\t To client : ", buff);
        // Methode zum Entleeren des Puffers
        bzero(buff, MAX);
        // Eingabe-Schleife, jedes Zeichen wird in den Puffer geschrieben bis 'Enter/Eingabetaste' gedrückt wird
        while ((buff[tmp++] = getchar()) != '\n');

        // Methode zum Schreiben an den Client und Speichern im Puffer
        write(connectionFileHandle, buff, sizeof(buff));

        /* Abfrage ob der Puffer nur aus dem Wort "exit" besteht,
         * mit der Methode "strncmp" welche 2 Strings vergleicht,
         * falls ja, dann wird die Schleife unterbrochen und die chatFunction Methode endet */
        if (strncmp(buff, "exit", 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

// Methode zum Erstellen des Server Sockets, sowie einer Verbindung
int main(){
    // Variablen zum Speichern der wichtigen Informationen des Sockets und der Connection
    int socketFileHandle, connectionFileHandle;
    // int length;
    // Strucutres für die Server Adresse und die Client Infos
    struct sockaddr_in serverAddress, client;
    //Leeren der Server Adresse
    bzero(&serverAddress, sizeof(serverAddress));

    // // Erstellen des Sockets, in diesem Fall TCP & Erfolg-/Fehler-Meldung
    socketFileHandle = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileHandle == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    // Zuteilung von IP-Adresse und Port
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(PORT);

    // Binden des Sockets an die IP-Adresse & Erfolg-/FehlerMeldung
    if ((bind(socketFileHandle, (SA*)&serverAddress, sizeof(serverAddress))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else {
        printf("Socket successfully binded..\n");
    }

    // listen öffnet den Server für alle Verbindungsanfragen, sodass der Client den Server finden kann & Erfolg-/FehlerMeldung
    if ((listen(socketFileHandle, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else {
        printf("Server listening..\n");
    }
    //length = sizeof(client); !!!!!!

    // Akzeptieren der Verbindungsanfrage des Clients mit accept und speichern der Verbindungsinfos im connectionFileHandle
    connectionFileHandle = accept(socketFileHandle, (SA*)&client, sizeof(&client));
    // Erfolg-/FehlerMeldung
    if (connectionFileHandle < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");

    // Aufruf der Chat Methode (s.o.)
    chatFunction(connectionFileHandle);

    // Schließen des Sockets
    close(socketFileHandle);
}
