#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Auswahl 1 des Menüs
void optionOne(){
    printf("Creating a TCP-Connection...\n----------\n");

    // Starten des Gegenprogramms auf einem zweiten Terminal
    system("gnome-terminal -- ./TCP/TCPServer");
    // Ausführung des Hauptprogramms auf dem gleichen Terminal
    system("./TCP/TCPClient");
}
// Auswahl 2 des Menüs
void optionTwo(){
    printf("Creating an UDP-Connection...\n----------\n");

    // Starten des Gegenprogramms auf einem zweiten Terminal
    system("gnome-terminal -- ./UDP/UDPServer");
    // Ausführung des Hauptprogramms auf dem gleichen Terminal
    system("./UDP/UDPClient");
}
// Auswahl 3 des Menüs
void optionThree(){
    printf("Opening LOG-FILES...\n----------\n");
    printf("Press [Q] to EXIT Log-File\n");
    // 2 Sekunden Wartezeit, zum Lesen der Informationsnachrichten
    sleep(2);
    system("sudo less tcpheaders.log");
}
// Auswahl 4 des Menüs
void optionFour(){
    printf("Opening LOG-FILES...\n----------\n");
    printf("Press [Q] to EXIT Log-File\n");
    // 2 Sekunden Wartezeit, zum Lesen der Informationsnachrichten
    sleep(2);
    system("sudo less udpheaders.log");
}

// Kleine Spielerei für ein freundlicheres GUI
void writeMenuHello(){
    printf("*       *  *********  **      *  *       * \n");
    printf("**     **  **         * *     *  *       * \n");
    printf("* *   * *  *          *  *    *  *       * \n");
    printf("*  * *  *  *********  *   *   *  *       * \n");
    printf("*   *   *  **         *    *  *  *       * \n");
    printf("*       *  *          *     * *   *     *  \n");
    printf("*       *  *********  *      **    *****   \n");
}

// Hauptteil des Programms, Verarbeitung und Steuerung der jeweiligen Eingabe
int main() {
    // Variable um den Input zu speichern
    int option;

    // Klassischer Methodenaufruf
    writeMenuHello();

    // Unendliche Schleife, damit das Menü nur per Exit-Option verlassen werden kann
    while (1) {
        // Prints für die verschiedenen verfügbaren Auswahlmöglichkeiten
        printf("Select an option:\n");
        printf("[1] Create TCP-Connection\n");
        printf("[2] Create UDP-Connection\n");
        printf("[3] Open LOG_FILES of Traffic for TCP\n");
        printf("[4] Open LOG_FILES of Traffic for UDP\n");
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
                optionOne();
                break;
            case 2:
                optionTwo();
                break;
            case 3:
                optionThree();
                break;
            case 4:
                optionFour();
                break;
            default:
                printf("Invalid option. Please select a valid option (1-4).\n");
                break;
        }
    }
}
