#include <stdio.h>
#include <stdlib.h>

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
    printf("Opening LOG_FILES...\n----------\n");
}
void optionFour(){
    printf("Option 4\n");
}
void optionFive(){
    printf("Option 5\n");
}
void optionExit(){
    printf("Exit\n");
}

int main() {
    int option;

    while (1) {
        printf("Select an option:\n");
        printf("[1] Create TCP-Connection\n");
        printf("[2] Create UDP-Connection\n");
        printf("[3] Open LOG_FILES of Traffic\n");
        printf("[4] Option 4\n");
        printf("[5] Option 5\n");
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
            case 5:
                optionFive();
                break;
            default:
                printf("Invalid option. Please select a valid option (1-5).\n");
                break;
        }
    }
    return 0;
}
