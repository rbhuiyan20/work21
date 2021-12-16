#include "pipe_networking.h"


int main() {

    int to_server;
    // start handshake
    int from_server;
     from_server = client_handshake( &to_server );
    char line[BUFFER_SIZE];

    while (1) {
        // Prompts user for input
        printf("Input please 0_0: ");
        fgets(line, BUFFER_SIZE, stdin);
        write(to_server, line, sizeof(line)); 
        read(from_server, line, sizeof(line)); 
        printf("Input in all UPPERCASE: %s\n", line); 
    }
}