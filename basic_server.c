#include "pipe_networking.h"


int main() {

    int to_client;
    int from_client;

    while (1) {
        // Repeatedly starts handshake
        from_client = server_handshake( &to_client );

        char line[BUFFER_SIZE];
        while (1) { // Prompt client for input
            if (read(from_client, line, sizeof(line))==0) { // reads data from client
                break;
            }

            // process string to all lower
            for(int i=0; i<strlen(line); i++) {
                line[i] = toupper(line[i]);
            }
            //send response to parent
            write(to_client, line, sizeof(line)); // send data to client
        }
    }
}