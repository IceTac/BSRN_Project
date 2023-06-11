#include <stdio.h>
#include <stdlib.h>
int main() {
    system("gnome-terminal -- ./TCP/TCPServer");
    system("./TCP/TCPClient");
    return 0;
}
