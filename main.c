#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void optionOne(){
    printf("Creating a TCP-Connection...\n----------\n");

    system("gnome-terminal -- ./TCP/TCPServer");
    system("./TCP/TCPClient");
}
void optionTwo(){
    printf("Creating an UDP-Connection...\n----------\n");

    system("gnome-terminal -- ./UDP/UDPServer");
    system("./UDP/UDPClient");
}
void optionThree(){
    printf("Opening LOG-FILES...\n----------\n");
    printf("Press [Q] to EXIT Log-File\n");
    sleep(2);
    system("sudo less tcpheaders.log");
}
void optionFour(){
    printf("Opening LOG-FILES...\n----------\n");
    printf("Press [Q] to EXIT Log-File\n");
    sleep(2);
    system("sudo less udpheaders.log");
}
void optionExit(){
    printf("Exit\n");
}
void writeMenuHello(){
    printf("*       *  *********  **      *  *       * \n");
    printf("**     **  **         * *     *  *       * \n");
    printf("* *   * *  *          *  *    *  *       * \n");
    printf("*  * *  *  *********  *   *   *  *       * \n");
    printf("*   *   *  **         *    *  *  *       * \n");
    printf("*       *  *          *     * *   *     *  \n");
    printf("*       *  *********  *      **    *****   \n");
}

int main() {
    int option;

    while (1) {
        writeMenuHello();
        //printf("Select an option:\n");
        printf("[1] Create TCP-Connection\n");
        printf("[2] Create UDP-Connection\n");
        printf("[3] Open LOG_FILES of Traffic for TCP\n");
        printf("[4] Open LOG_FILES of Traffic for UDP\n");
        printf("[0] To EXIT\n");

        if (scanf("%d", &option) != 1) {
            printf("Invalid input. Please enter a number.\n");
            // Clear the input buffer
            while (getchar() != '\n');
            continue;
        }
        if(option == 0){
            optionExit();
            break;
        }

        switch (option) {
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
                printf("Invalid option. Please select a valid option (1-3).\n");
                break;
        }
    }
    return 0;
}
