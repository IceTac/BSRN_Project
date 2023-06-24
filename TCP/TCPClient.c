#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strncmp()
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

// Festlegung der maximalen Zeichen-Anzahl pro Nachricht
#define MAX 80
// Festlegung des Ports
#define PORT 8080
// Festlegung des Kürzels SA (Socket Adress) für die Structure sockaddr [Nur für Convenience]
#define SA struct sockaddr

// Methode welche den File Handle vom Socket als Parameter für read() und write() benutzt
void chatFunction(int socketFileHandle){
    // Erstellung eines Puffers der Größe 80 (s.o.)
    char buff[MAX];

    // Zähler Variable tmp, später nur wichtig für die Eingabe in den Puffer
    int tmp;
    // Endlos-Schleife, die nur bei Bedinugnen abbricht, damit Chat endlos weitergehen kann
    for (;;) {
        // Pro Durchang wird tmp auf 0 gesetzt
        tmp = 0;
        // Methode zum Entleeren des Puffers (für fehlerfreie, dopplungsfreie Übertragung wichtig)
        bzero(buff, MAX);
        // Eingabeaufforderung an den Nutzer
        printf("Enter the string : ");

        // Eingabe-Schleife, jedes Zeichen wird in den Puffer geschrieben bis 'Enter/Eingabetaste' gedrückt wird
        while ((buff[tmp++] = getchar()) != '\n');

        // Methode zum Schreiben an den Server
        write(socketFileHandle, buff, sizeof(buff));
        /* Abfrage ob der Puffer nur aus dem Wort "exit" besteht,
         * mit der Methode "strncmp" welche 2 Strings vergleicht,
         * falls ja dann wird die Schleife unterbrochen und die chatFunction Methode endet */
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
        // Methode zum Entleeren des Puffers
        bzero(buff, MAX);

        // Methode zum Lesen vom Server
        read(socketFileHandle, buff, sizeof(buff));
        printf("From Server : %s\t", buff);
        // Abfrage ob exit das Wort im Puffer ist ... (s.o.)
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}
// Erstellen eines Sockets und Erstellen einer Verbindung zum Server
void creatingConnection(){
    // Der Socket-Datei-Verarbeiter (Hauptspeicher Variable des Sockets)
    int socketFileHandle;
    // Eine Structure für die Server Adresse
    struct sockaddr_in serverAdress;
    //Leeren der Server Adresse, da selbst eine IP festgelegt wird
    bzero(&serverAdress, sizeof(serverAdress));

    // Erstellen des Sockets mit dem default Protokoll, in diesem Fall TCP & Erfolg-/Fehler-Meldung
    socketFileHandle = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileHandle == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else {
        printf("Socket successfully created..\n");
    }

    // Eingabe der IP-Adresse vom Nutzer
    // Variable für die IP-Adresse
    char ipAdress[20];
    printf("Please enter an IP-Adress:\n");
    // Standard Eingabe durch scanf
    scanf("%s", ipAdress);
    //Leeren des Input-Puffers für kommende Eingaben im Chat-Bereich
    while (getchar() != '\n');

    // Zuteilung von IP-Adresse und Port
    serverAdress.sin_family = AF_INET;
    serverAdress.sin_addr.s_addr = inet_addr(ipAdress);
    serverAdress.sin_port = htons(PORT);

    // Verbindung mit dem Server durch connect, dem socketFileHandle, der Server Adresse und Socket Adresse (SA) & Erfolg-/Fehler-Meldung
    if (connect(socketFileHandle, (SA*)&serverAdress, sizeof(serverAdress)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else {
        printf("connected to the server..\n");
    }

    // Aufruf der Chat Methode (s.o.)
    chatFunction(socketFileHandle);

    // Schließen des Sockets
    close(socketFileHandle);
}

// Methode um Name, Port und Protokoll eines Services herauszufinden
void serviceInformation(){
    // Variable für den Namen der vom Nutzer eingegeben wird
    char serviceName[20];
    printf("Please enter a Service:\n");
    // Standard Eingabe
    scanf("%s", serviceName);
    // Structure um die getservbyname Methode auszuführen und die Rückgabewerte zu speichern
    struct servent *service = getservbyname(serviceName, "tcp");
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
        printf("[1] Start the TCP-Client\n");
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
                creatingConnection();
                break;
            case 2:
                // Aufruf eines neuen Terminals für das PCAP-Programm
                system("gnome-terminal -- sudo ./pcapForTCP");
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
