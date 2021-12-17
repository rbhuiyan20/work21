#include "pipe_networking.h"


int main() {

    int to_client;
    int from_client;

    while (1) {
        // Constantly does the handshake
        from_client = server_handshake( &to_client );

        char userIn[BUFFER_SIZE];
        while (1) { 
            // Reads in the user input
            if (read(from_client, userIn, sizeof(userIn))==0) { 
                break;
            }

            // makes user input all uppercase
            for(int i=0; i<strlen(userIn); i++) {
                userIn[i] = toupper(userIn[i]);
            }
            //send data back to parent
            write(to_client, userIn, sizeof(userIn));
        }
    }
}
