#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>

// Festlegung des Ports
#define PORT 5000
// Festlegung der maximalen Zeichen-Anzahl pro Nachricht
#define MAX 80

// Methode zum Erstellen und laufen lassen des Clients
void runningChatFunction(){
    // Erstellung eines Puffers der Größe 80 (s.o.)
    char buff[MAX];
    // Der Socket-Datei-Verarbeiter (Hauptspeicher Variable des Sockets)
    int socketFileHandle;
    // Eine Structure für die Server Adresse
    struct sockaddr_in serverAddress;

    // Die Structure der Server Adresse wird geleert
    bzero(&serverAddress, sizeof(serverAddress));

    // Eingabe der IP-Adresse vom Nutzer
    // Variable für die IP-Adresse
    char ipAddress[20];
    printf("Please enter an IP-Adress:\n");
    // Standard Eingabe durch scanf
    scanf("%s", ipAddress);
    //Leeren des Input-Puffers für kommende Eingaben im Chat-Bereich
    while (getchar() != '\n');

    // Zuteilung von IP-Adresse und Port
    serverAddress.sin_addr.s_addr = inet_addr(ipAddress);
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_family = AF_INET;

    // Erstellen des Sockets, in diesem Fall UDP & Erfolg-/Fehler-Meldung
    socketFileHandle = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFileHandle == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    // Verbindung mit dem Server durch connect, dem socketFileHandle, der Server Adresse und Socket Adresse & Erfolg-/Fehler-Meldung
    if(connect(socketFileHandle, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
        printf("Error: Connect Failed \n");
        exit(0);
    }else
        printf("Connection with Server successful\n");

    // Endlos-Schleife, die nur bei Bedingungen abbricht, damit Chat endlos weitergehen kann
    for(;;) {
        // Zähler Variable tmp, später nur wichtig für die Eingabe in den Puffer, wird pro Durchgang auf 0 gesetzt
        int tmp = 0;

        // Leeren des Puffers
        bzero(buff, MAX);

        printf("To Server: ");
        // Eingabe-Schleife, jedes Zeichen wird in den Puffer geschrieben bis 'Enter/Eingabetaste' gedrückt wird
        while ((buff[tmp++] = getchar()) != '\n');
        // Senden der Nachricht an den Server
        sendto(socketFileHandle, buff, MAX, 0, (struct sockaddr *) NULL, sizeof(serverAddress));
        /* Abfrage ob der Puffer nur aus dem Wort "exit" besteht,
         * mit der Methode "strncmp" welche 2 Strings vergleicht,
         * falls ja, dann wird die Schleife unterbrochen und die chatFunction Methode endet */
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }

        // Leeren des Puffers
        bzero(buff, MAX);

        // Empfangen der Nachricht des Servers
        recvfrom(socketFileHandle, buff, sizeof(buff), 0, (struct sockaddr *) NULL, NULL);
        printf("From Server: %s\n", buff);
    }
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
    // Abfrage um zu gucken, ob es Infos zum eingegebenen Host gibt oder nicht, Ausgabe & Fehler-Meldung
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

    // Unendliche Schleife, damit das Menü nur per Exit-Option verlassen werden kann
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
