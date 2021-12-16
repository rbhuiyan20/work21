#include "pipe_networking.h"

// self reminder- diagnostic print messages
/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {

    printf("CREATING WKP\n");
    int err = mkfifo(WKP, 0644);
    if(err == -1) {
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    printf("RECIEVING PRIVATE FIFO NAME\n");
    // from client
    int to_server= open(WKP, O_RDONLY);
    if(to_server== -1) {
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    char message[HANDSHAKE_BUFFER_SIZE];
    printf("READING WKP\n");
    err = read(to_server, message, HANDSHAKE_BUFFER_SIZE);
    if(err == -1) {
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    printf("REMOVE WKP\n");
    remove(WKP);

    printf("CLIENT GIVING RESPONSE\n");
    *to_client = open(message, O_WRONLY);
    if(*to_client == -1) {
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    printf("SENDING ACK TO CLIENT\n");
    err = write(*to_client, ACK, sizeof(ACK));
    if(err == -1) {
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    printf("CLIENT GIVES RESPONSE\n");
    err = read(to_server, message, sizeof(message));
    if(err == -1) {
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    if(strcmp(message, ACK) != 0) {
        printf("HANDSHAKE FAILED\n");
        return 0;
    }
    printf("HANDSHAKE WORKED\n");
    return to_server;
}
/*=========================
client_handshake
args: int * to_server
Performs the client side pipe 3 way handshake.
Sets *to_serverto the file descriptor for the upstream pipe.
returns the file descriptor for the downstream pipe.
=========================*/
int client_handshake(int *to_server) {

    char pid[1000];
    printf("CREATING FIFO\n");
    sprintf(pid, "%d", getpid() );
    mkfifo(pid, 0644);

    printf("OPENING WKP\n");
    *to_server= open(WKP, O_WRONLY);
    if (*to_server== -1) {
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    printf("SENDING NAME OF FIFO\n");
    int err = write(*to_server, pid, HANDSHAKE_BUFFER_SIZE);
    if (err == -1) {
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    printf("RECIEVING ACK\n");
    int from_server = open(pid, O_RDONLY);
    if (err == -1) {
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    printf("READING ACK FROM SERVER\n");
    char message[HANDSHAKE_BUFFER_SIZE];
    err = read(from_server, message, HANDSHAKE_BUFFER_SIZE);

    printf("GETTING RID OF PIPE\n");
    remove(pid);

    printf("Sending response to server...\n");
    err = write(*to_server, ACK, sizeof(ACK));
    if (err == -1) {
        printf("Error: %s\n", strerror(errno));
        return 0;
    }
    return from_server;
}